#include "gfx_internal.hpp"

#include <GLES3/gl3.h>

namespace gfx {

// namespace {
// constexpr GLfloat VERTICES[] = {
//     // positions        // hue
//     -0.5f, -0.5f, 0.0f, 0.0f,     //
//     0.5f,  -0.5f, 0.0f, 0.33333f, //
//     0.0f,  0.5f,  0.0f, 0.66667f};
// }

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
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

  // NOTE: Position attrs
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);
  // NOTE: Hue attrs
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  return VertexBuffer{id};
}

void VertexBuffer::bind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, id); }
void VertexBuffer::unbind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::update(const Vec3f &pos) noexcept {
  const GLfloat p[] = {pos.x, pos.y, pos.z};
  glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLfloat), p);
}

} // namespace gfx
