#include "geom.hpp"
#include "gfx_internal.hpp"
#include "gpu.hpp"

#include <gsb/config.h>
#include <gsb/gfx.hpp>

#include <GLES3/gl3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <cassert>
#include <cstddef>
#include <expected>
#include <format>
#include <memory>
#include <string>
#include <utility>

namespace gfx {

#include <gsb/shaders/frag.glsl.hpp>
#include <gsb/shaders/vert.glsl.hpp>

struct Renderer::Impl {
  gpu::Mesh mesh;
  gpu::Batch batch;
  ShaderProg shp;
  Camera cam;
  glm::mat4 proj;

  Impl(const geom::Mesh &g, std::size_t max, ShaderProg &&s) noexcept
      : mesh{g}, batch{mesh, max}, shp{std::move(s)}, cam{{0, 0, 300.f}},
        proj{glm::perspective(glm::radians(PERSPECTIVE_FOV), ASPECT_RATIO,
                              PERSPECTIVE_NEAR, PERSPECTIVE_FAR)} {}
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

void Renderer::draw_sphere(const glm::vec4 &xyzr) noexcept {
  impl->batch.push(xyzr);
}

void Renderer::present(double delta_time) noexcept {
  impl->shp.use();
  impl->shp.update(delta_time);
  impl->cam.update(delta_time);
  impl->shp.view(impl->cam.view());
  impl->shp.proj(impl->proj);
  impl->batch.flush();
}

}; // namespace gfx
