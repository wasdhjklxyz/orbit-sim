#include "gfx.hpp"
#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <expected>
#include <format>
#include <memory>
#include <utility>

namespace gfx {

namespace {

const char *VERTEX_SHADER_SRC =
    "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }";

const char *FRAGMENT_SHADER_SRC =
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{ FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); }";

} // namespace

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
  auto sh_r = Shader::compile(GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
  if (!sh_r) {
    return std::unexpected{std::format("Vertex shader: {}", sh_r.error())};
  }
  Shader vsh = std::move(*sh_r);

  sh_r = Shader::compile(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
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
  vbo.bind();
  vao.unbind();

  return Renderer{std::make_unique<Impl>(std::move(vao), std::move(vbo),
                                         std::move(*shp_r))};
}

void Renderer::clear() noexcept { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::draw() const noexcept {
  impl->shp.use();
  impl->vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  impl->vao.unbind();
}

}; // namespace gfx
