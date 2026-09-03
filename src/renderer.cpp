#include "gfx.hpp"
#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <cassert>
#include <expected>
#include <format>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <utility>

#include "config.h"
#include "geom.hpp"
#include "gpu.hpp"

namespace gfx {

#include "shaders/frag.glsl.hpp"
#include "shaders/vert.glsl.hpp"

struct Renderer::Impl {
  gpu::Mesh mesh;
  gpu::Batch batch;
  ShaderProg shp;
  glm::mat4 proj;

  Impl(const geom::Mesh &g, std::size_t max, ShaderProg &&s) noexcept
      : mesh{g}, batch{mesh, max}, shp{std::move(s)},
        proj{glm::ortho(-METERS_WIDTH / 2.f, METERS_WIDTH / 2.f,
                        -METERS_HEIGHT / 2.f, METERS_HEIGHT / 2.f, -1000.f,
                        1000.f)} {}
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

  static const geom::Sphere sphere{};
  return Renderer{
      std::make_unique<Impl>(sphere, MAX_ENTITIES, std::move(*shp_r))};
}

void Renderer::clear() noexcept {
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw_circle(const glm::vec4 &xyzr) noexcept {
  impl->batch.push(xyzr);
}

void Renderer::present(double delta_time) noexcept {
  impl->shp.use();
  impl->shp.update(delta_time);
  impl->shp.proj(impl->proj);
  impl->batch.flush();
}

}; // namespace gfx
