#include "gfx.hpp"
#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <cassert>
#include <expected>
#include <format>
#include <memory>
#include <numbers>
#include <utility>
#include <vector>

namespace gfx {

#include "shaders/frag.glsl.hpp"
#include "shaders/vert.glsl.hpp"

constexpr float METERS_PER_NDC = 200; // NOTE: World units per half screen
constexpr float ASPECT_RATIO = float(720) / float(1280); // FIXME: Refactor

static std::vector<Vec3f> mk_unit_circle(std::size_t n) {
  constexpr float TAU = 2.0 * std::numbers::pi_v<double>;
  std::vector<Vec3f> uc(n);
  for (std::size_t i = 0; i < n; i++) {
    const float t = TAU * i / n;
    uc[i] = Vec3f{std::cos(t), std::sin(t), 0.0};
  }
  return uc;
}
static const auto UNIT_CIRCLE = mk_unit_circle(NUM_VERTICES_CIRCLE - 2);

struct Renderer::Impl {
  VertexArray vao;
  VertexBuffer vbo;
  ShaderProg shp;
};

Renderer::Renderer(std::unique_ptr<Impl> p) noexcept : impl{std::move(p)} {}
Renderer::Renderer(Renderer &&) noexcept = default;
Renderer &Renderer::operator=(Renderer &&) noexcept = default;
Renderer::~Renderer() = default;

std::expected<Renderer, std::string> Renderer::create() {
  auto sh_r = Shader::compile(GL_VERTEX_SHADER, shaders::vert_glsl);
  if (!sh_r) {
    return std::unexpected{std::format("Vertex shader: {}", sh_r.error())};
  }
  Shader vsh = std::move(*sh_r);

  sh_r = Shader::compile(GL_FRAGMENT_SHADER, shaders::frag_glsl);
  if (!sh_r) {
    return std::unexpected{std::format("Fragment shader: {}", sh_r.error())};
  }
  Shader fsh = std::move(*sh_r);

  auto shp_r = ShaderProg::create(vsh, fsh);
  if (!shp_r) {
    return std::unexpected{std::format("Shader program: {}", shp_r.error())};
  }

  // TODO: Refactor all this VBO/VAO creation stuff
  VertexArray vao = VertexArray::create();
  VertexBuffer vbo = VertexBuffer::create();
  vao.unbind();

  return Renderer{std::make_unique<Impl>(std::move(vao), std::move(vbo),
                                         std::move(*shp_r))};
}

void Renderer::clear() noexcept { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::draw_circle(const Vec3f &center, const float radius) noexcept {
  const auto center_ndc = center / METERS_PER_NDC;
  std::array<Vec3f, NUM_VERTICES_CIRCLE> buf{center_ndc};

  // FIXME: Refactor the NDC scale of unit circle, dont
  //        care now since this is inefficient anyways
  for (std::size_t i = 0; i < NUM_VERTICES_CIRCLE - 2; i++)
    buf[i + 1] = center_ndc + radius * (UNIT_CIRCLE[i] / METERS_PER_NDC);
  buf[NUM_VERTICES_CIRCLE - 1] = buf[1];
  for (std::size_t i = 0; i < buf.size(); i++)
    buf[i].x *= ASPECT_RATIO;

  impl->vbo.push(buf);
}

void Renderer::present(const float deltaTime) noexcept {
  impl->shp.use();
  impl->shp.update(deltaTime);
  impl->vbo.bind();
  const auto count = impl->vbo.flush();
  impl->vao.bind();
  assert(count % NUM_VERTICES_CIRCLE == 0);
  // FIXME: See glDrawArraysInstanced this is dumb
  for (std::size_t i = 0; i < count; i += NUM_VERTICES_CIRCLE)
    glDrawArrays(GL_TRIANGLE_FAN, i, NUM_VERTICES_CIRCLE);
  impl->vao.unbind();
}

}; // namespace gfx
