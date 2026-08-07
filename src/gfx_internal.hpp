#pragma once

#include <GLES3/gl3.h>

#include <concepts>
#include <expected>
#include <span>
#include <string>
#include <string_view>

namespace gfx {

class VertexBuffer {
public:
  ~VertexBuffer() noexcept;
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;
  VertexBuffer(VertexBuffer &&) noexcept;
  VertexBuffer &operator=(VertexBuffer &&) noexcept;

  static VertexBuffer create();
  void bind() noexcept;

private:
  explicit VertexBuffer(GLuint) noexcept;
  GLuint id;
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
