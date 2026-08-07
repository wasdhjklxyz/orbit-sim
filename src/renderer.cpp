#include "gfx.hpp"
#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <expected>
#include <format>
#include <memory>
#include <utility>

namespace gfx {

#include "shaders/frag.glsl.hpp"
#include "shaders/vert.glsl.hpp"

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

void Renderer::draw(const float deltaTime, const Vec3f &pos) noexcept {
  impl->shp.use();
  impl->shp.update(deltaTime);
  impl->vbo.bind();
  impl->vbo.update(pos);
  impl->vao.bind();
  glDrawArrays(GL_POINTS, 0, 1);
  impl->vao.unbind();
}

}; // namespace gfx
