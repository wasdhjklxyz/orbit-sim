#include "gpu.hpp"

#include "geom.hpp"

#include <GLES3/gl3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/vec4.hpp>

#include <cassert>
#include <cstddef>
#include <span>
#include <utility>
#include <vector>

/* TODO: Error handling */

namespace gfx::gpu {

Array::Array() noexcept { glGenVertexArrays(1, &id); }
Array::~Array() noexcept { glDeleteVertexArrays(1, &id); }
Array::Array(Array &&other) noexcept : id{std::exchange(other.id, 0)} {}
Array &Array::operator=(Array &&other) noexcept {
  if (this != &other) {
    glDeleteVertexArrays(1, &id);
    id = std::exchange(other.id, 0);
  }
  return *this;
}

void Array::bind() const noexcept { glBindVertexArray(id); }
void Array::unbind() const noexcept { glBindVertexArray(0); }

Buffer::Buffer(GLenum target, GLenum usage) noexcept
    : target{target}, usage{usage} {
  glGenBuffers(1, &id);
}
Buffer::~Buffer() noexcept { glDeleteBuffers(1, &id); }
Buffer::Buffer(Buffer &&other) noexcept
    : target{other.target}, usage{other.usage}, id{std::exchange(other.id, 0)} {
}
Buffer &Buffer::operator=(Buffer &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &id);
    target = other.target;
    usage = other.usage;
    id = std::exchange(other.id, 0);
  }
  return *this;
}

void Buffer::bind() const noexcept { glBindBuffer(target, id); }
void Buffer::unbind() const noexcept { glBindBuffer(target, 0); }

template <BufferSource BS> void Buffer::set(BS &&src) noexcept {
  bind();
  std::span data{std::forward<BS>(src)};
  glBufferData(target, data.size_bytes(), data.data(), usage);
}

void Buffer::reserve(std::size_t bytes) noexcept {
  bind();
  glBufferData(target, bytes, nullptr, usage);
}

template <BufferSource BS> void Buffer::update(BS &&src) noexcept {
  bind();
  std::span data{std::forward<BS>(src)};
  glBufferSubData(target, 0, data.size_bytes(), data.data());
}

Mesh::Mesh(const geom::Mesh &geom) noexcept
    : vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW},
      ibo{GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW},
      index_count_{static_cast<GLuint>(geom.indices().size())} {
  vbo.set(geom.vertices());
  ibo.set(geom.indices());
  // FIXME: Unbind? Reserve at all with set?
}

void Mesh::bind_attrs() const noexcept {
  ibo.bind();
  vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
  glEnableVertexAttribArray(0);
}

Batch::Batch(const Mesh &mesh, std::size_t max_instances) noexcept
    : mesh{mesh}, instances{GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW},
      max_instances{max_instances} {
  staging.reserve(max_instances);
  instances.reserve(max_instances * sizeof(glm::vec4));
  vao.bind();
  mesh.bind_attrs();
  instances.bind();
  glVertexAttribPointer(1, glm::vec4::length(), GL_FLOAT, GL_FALSE,
                        sizeof(glm::vec4), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);
  vao.unbind();
}

void Batch::push(glm::vec4 inst) noexcept {
  if (staging.size() > max_instances)
    return;
  staging.push_back(inst);
}

void Batch::flush() noexcept {
  if (staging.empty())
    return;
  instances.update(staging);
  vao.bind();
  glDrawElementsInstanced(GL_TRIANGLES, mesh.index_count(), GL_UNSIGNED_INT,
                          nullptr, static_cast<GLsizei>(staging.size()));
  vao.unbind();
  staging.clear();
}

} // namespace gfx::gpu
