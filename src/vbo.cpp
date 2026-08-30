#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <cassert>
#include <vector>

namespace gfx {

VertexBuffer::VertexBuffer(GLuint id, std::size_t max_size,
                           std::vector<Vec3f> &&buf) noexcept
    : id{id}, max_size{max_size}, buf{buf} {}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : id{std::exchange(other.id, 0)},
      max_size{std::exchange(other.max_size, 0)}, buf{other.buf} {}

VertexBuffer::~VertexBuffer() noexcept {
  // TODO: Unbind?
  glDeleteBuffers(1, &id);
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &id);
    id = std::exchange(other.id, 0);
    max_size = std::exchange(other.max_size, 0);
    buf = std::move(other.buf);
  }
  return *this;
}

VertexBuffer VertexBuffer::create(std::size_t max_vertices) {
  const auto size = max_vertices * 3 * sizeof(GLfloat);
  GLuint id;

  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);

  std::vector<Vec3f> buf;
  buf.reserve(size);

  return VertexBuffer{id, size, std::move(buf)};
}

void VertexBuffer::bind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, id); }
void VertexBuffer::unbind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::push(std::span<Vec3f> data) noexcept {
  buf.append_range(data);
}

std::size_t VertexBuffer::flush() noexcept {
  const auto size = sizeof(GLfloat) * 3 * buf.size();
  assert(max_size >= size); // TODO: >= or > ?
  glBufferData(GL_ARRAY_BUFFER, size, buf.data(), GL_DYNAMIC_DRAW);
  buf.clear();
  return size;
}

} // namespace gfx
