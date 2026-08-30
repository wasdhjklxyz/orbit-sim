#pragma once

#include <GLES3/gl3.h>

#include <concepts>
#include <expected>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "vec.hpp"

namespace gfx {

constexpr std::size_t NUM_VERTICES_CIRCLE = 32;
static_assert(NUM_VERTICES_CIRCLE >= 3);

class VB {
public:
private:
  std::vector<Vec3f> buf;
};

class VertexBuffer {
public:
  ~VertexBuffer() noexcept;
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;
  VertexBuffer(VertexBuffer &&) noexcept;
  VertexBuffer &operator=(VertexBuffer &&) noexcept;

  static VertexBuffer
  create(std::size_t num_vertices = GL_MAX_ELEMENTS_VERTICES);
  void bind() noexcept;
  void unbind() noexcept;
  void update(const Vec3d &pos, const double radius) noexcept;
  void push(std::span<Vec3f> data) noexcept;
  std::size_t flush() noexcept;

private:
  explicit VertexBuffer(GLuint id, std::size_t max_size,
                        std::vector<Vec3f> &&buf) noexcept;
  GLuint id;
  std::size_t max_size;
  std::vector<Vec3f> buf;
};

class VertexArray {
public:
  ~VertexArray() noexcept;
  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;
  VertexArray(VertexArray &&) noexcept;
  VertexArray &operator=(VertexArray &&) noexcept;

  static VertexArray create();
  void bind() noexcept;
  static void unbind() noexcept;

private:
  explicit VertexArray(GLuint) noexcept;
  GLuint id;
};

class Shader {
public:
  ~Shader() noexcept;
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;
  Shader(Shader &&) noexcept;
  Shader &operator=(Shader &&) noexcept;

  static std::expected<Shader, std::string> compile(GLenum, std::string_view);
  GLuint get() const noexcept;

private:
  explicit Shader(GLuint) noexcept;
  std::string getInfoLog() const;

  GLuint shid{0};
};

class ShaderProg {
public:
  ~ShaderProg() noexcept;
  ShaderProg(const ShaderProg &) = delete;
  ShaderProg &operator=(const ShaderProg &) = delete;
  ShaderProg(ShaderProg &&) noexcept;
  ShaderProg &operator=(ShaderProg &&) noexcept;

  template <std::same_as<Shader>... Ss>
    requires(sizeof...(Ss) >= 2) // NOTE: Vertex and fragment shaders required
  static std::expected<ShaderProg, std::string> create(const Ss &...shaders) {
    const GLuint ids[]{shaders.get()...};
    return createImpl(ids);
  }
  void use() const noexcept;
  void update(const float deltaTime) noexcept;

private:
  explicit ShaderProg(GLuint) noexcept;
  static std::expected<ShaderProg, std::string>
      createImpl(std::span<const GLuint>);
  std::string getInfoLog() const;

  GLuint spid{0};
  float elapsed{0};
};

} // namespace gfx
