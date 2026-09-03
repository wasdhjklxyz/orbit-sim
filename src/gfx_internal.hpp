#pragma once

#include <GLES3/gl3.h>

#include <concepts>
#include <expected>
#include <glm/mat4x4.hpp>
#include <span>
#include <string>
#include <string_view>

namespace gfx {

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
  void update(double delta_time) noexcept;
  void proj(const glm::mat4 &m) const noexcept;

private:
  explicit ShaderProg(GLuint) noexcept;
  static std::expected<ShaderProg, std::string>
      createImpl(std::span<const GLuint>);
  std::string getInfoLog() const;

  GLuint spid{0};
  float elapsed{0};
};

} // namespace gfx
