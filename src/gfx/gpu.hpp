#pragma once

#include "geom.hpp"

#include <GLES3/gl3.h>
#include <glm/ext/vector_float4.hpp>
#include <glm/vec4.hpp>

#include <cstddef>
#include <ranges>
#include <type_traits>
#include <vector>

namespace gfx::gpu {

class Array {
public:
  explicit Array() noexcept;
  ~Array() noexcept;
  Array(const Array &) = delete;
  Array &operator=(const Array &) = delete;
  Array(Array &&) noexcept;
  Array &operator=(Array &&) noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

private:
  GLuint id;
};

template <typename T>
concept BufferSource =
    std::ranges::contiguous_range<T> &&
    std::is_trivially_copyable_v<std::ranges::range_value_t<T>>;

class Buffer {
public:
  explicit Buffer(GLenum target, GLenum usage) noexcept;
  ~Buffer() noexcept;
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer(Buffer &&) noexcept;
  Buffer &operator=(Buffer &&) noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;
  template <BufferSource BS> void set(BS &&src) noexcept;
  void reserve(std::size_t bytes) noexcept;
  template <BufferSource BS> void update(BS &&src) noexcept;

private:
  GLenum target;
  GLenum usage;
  GLuint id;
};

class Mesh {
public:
  explicit Mesh(const geom::Mesh &geom) noexcept;

  void bind_attrs() const noexcept;
  GLuint index_count() const noexcept { return index_count_; }

private:
  Buffer vbo;
  Buffer ibo;
  GLuint index_count_;
};

class Batch {
public:
  explicit Batch(const Mesh &mesh, std::size_t max_instances) noexcept;

  void push(glm::vec4 inst) noexcept;
  void flush() noexcept; // NOTE: upload + draw

private:
  const Mesh &mesh;
  Array vao;
  Buffer instances; // NOTE: GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW
  std::size_t max_instances;
  std::vector<glm::vec4> staging; // NOTE: (cx, cy, cz, radius)
};

}; // namespace gfx::gpu
