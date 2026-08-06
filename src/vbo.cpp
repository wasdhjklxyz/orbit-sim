#include "gfx_internal.hpp"

#include <GLES3/gl3.h>

namespace gfx {

namespace {
constexpr GLfloat VERTICES[] = {
    // positions        // colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f};
}

VertexBuffer::VertexBuffer(GLuint id) noexcept : id{id} {}
VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : id{std::exchange(other.id, 0)} {}

VertexBuffer::~VertexBuffer() noexcept {
  // TODO: Unbind?
  glDeleteBuffers(1, &id);
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &id);
    id = std::exchange(other.id, 0);
  }
  return *this;
}

VertexBuffer VertexBuffer::create() {
  // TODO: Error handling/checking, also fix hardcoded values lol
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

  // NOTE: Position attrs
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);
  // NOTE: Color attrs
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  return VertexBuffer{id};
}

void VertexBuffer::bind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, id); }

} // namespace gfx
