#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include <GLES3/gl3.h>
#include <concepts>
#include <expected>
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/mat4x4.hpp>
#include <span>
#include <string>
#include <string_view>

namespace gfx {

inline constexpr glm::vec3 WORLD_UP = {0.f, 1.f, 0.f};

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
  void view(const glm::mat4 &m) const noexcept;

private:
  explicit ShaderProg(GLuint) noexcept;
  static std::expected<ShaderProg, std::string>
      createImpl(std::span<const GLuint>);
  std::string getInfoLog() const;

  GLuint spid{0};
  float elapsed{0};
};

class Camera {
public:
  explicit Camera(glm::vec3 pos, float pitch = 0.f, float yaw = 180.f) noexcept;

  void move(const glm::vec3 &dir, float pitch, float yaw) noexcept;
  void update(double delta_time) noexcept;

  inline constexpr glm::vec3 forward() const noexcept { return dir; }
  inline constexpr glm::vec3 right() const noexcept { return right_; }
  inline constexpr glm::mat4 view() const noexcept {
    return glm::lookAt(pos, pos + dir, up);
  }

private:
  glm::vec3 pos, vel;
  glm::vec3 dir, right_, up;
};

} // namespace gfx
