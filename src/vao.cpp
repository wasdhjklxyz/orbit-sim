#include "gfx_internal.hpp"

#include <GLES3/gl3.h>

namespace gfx {

VertexArray::VertexArray(GLuint id) noexcept : id{id} {}
VertexArray::VertexArray(VertexArray &&other) noexcept
    : id{std::exchange(other.id, 0)} {}

VertexArray::~VertexArray() noexcept {
  // TODO: Unbind?
  glDeleteVertexArrays(1, &id);
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
  if (this != &other) {
    glDeleteVertexArrays(1, &id);
    id = std::exchange(other.id, 0);
  }
  return *this;
}

VertexArray VertexArray::create() {
  // TODO: Error handling/checking
  GLuint id;
  glGenVertexArrays(1, &id);
  glBindVertexArray(id);
  return VertexArray{id};
}

} // namespace gfx
