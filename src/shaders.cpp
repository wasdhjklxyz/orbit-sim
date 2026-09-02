#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <format>
#include <glm/mat4x4.hpp>

namespace gfx {

namespace {

std::string readLog(GLuint id, void (*getiv)(GLuint, GLenum, GLint *),
                    void (*getlog)(GLuint, GLsizei, GLsizei *, GLchar *)) {
  GLint len{0};
  getiv(id, GL_INFO_LOG_LENGTH, &len);
  if (len <= 0) {
    return "(no info log)";
  }
  std::string log(static_cast<size_t>(len), '\0');
  GLsizei wtn{0};
  getlog(id, len, &wtn, log.data());
  log.resize(static_cast<size_t>(wtn));
  return log;
}

} // namespace

Shader::Shader(GLuint shid) noexcept : shid{shid} {}
Shader::~Shader() noexcept { glDeleteShader(shid); }
Shader::Shader(Shader &&other) noexcept : shid{std::exchange(other.shid, 0)} {}

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    glDeleteShader(shid);
    shid = std::exchange(other.shid, 0);
  }
  return *this;
}

GLuint Shader::get() const noexcept { return shid; }

std::string Shader::getInfoLog() const {
  return readLog(shid, glGetShaderiv, glGetShaderInfoLog);
}

std::expected<Shader, std::string> Shader::compile(GLenum type,
                                                   std::string_view src) {
  Shader sh{glCreateShader(type)};
  if (sh.shid == 0) {
    return std::unexpected{"glCreateShader: error"};
  }

  const char *data = src.data();
  const GLint len = static_cast<GLint>(src.size());
  glShaderSource(sh.shid, 1, &data, &len);
  glCompileShader(sh.shid);

  GLint ok{0};
  glGetShaderiv(sh.shid, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    return std::unexpected{
        std::format("Shader compilation failed: {}", sh.getInfoLog())};
  }
  return sh;
}

ShaderProg::ShaderProg(GLuint spid) noexcept : spid{spid} {}
ShaderProg::~ShaderProg() noexcept { glDeleteProgram(spid); }
ShaderProg::ShaderProg(ShaderProg &&other) noexcept
    : spid{std::exchange(other.spid, 0)} {}

ShaderProg &ShaderProg::operator=(ShaderProg &&other) noexcept {
  if (this != &other) {
    glDeleteProgram(spid);
    spid = std::exchange(other.spid, 0);
  }
  return *this;
}

void ShaderProg::use() const noexcept { glUseProgram(spid); }

std::string ShaderProg::getInfoLog() const {
  return readLog(spid, glGetProgramiv, glGetProgramInfoLog);
}

std::expected<ShaderProg, std::string>
ShaderProg::createImpl(std::span<const GLuint> shids) {
  for (std::size_t i = 0; i < shids.size(); ++i) {
    if (shids[i] == 0) {
      return std::unexpected{std::format("Invalid shader at index {}", i)};
    }
    for (std::size_t j = i + 1; j < shids.size(); ++j) {
      if (shids[i] == shids[j]) {
        return std::unexpected{
            std::format("Duplicate shader at indices {} and {}", i, j)};
      }
    }
  }

  ShaderProg sp{glCreateProgram()};
  if (sp.spid == 0) {
    return std::unexpected{"glCreateProgram: error"};
  }

  for (const GLuint id : shids) {
    glAttachShader(sp.spid, id);
  }
  glLinkProgram(sp.spid);
  for (const GLuint id : shids) {
    glDetachShader(sp.spid, id);
  }

  GLint ok{0};
  glGetProgramiv(sp.spid, GL_LINK_STATUS, &ok);
  if (!ok) {
    return std::unexpected{
        std::format("Shader program linking failed: {}", sp.getInfoLog())};
  }
  return sp;
}

void ShaderProg::update(const float deltaTime) noexcept {
  GLint loc = glGetUniformLocation(spid, "uTime");
  elapsed += deltaTime;
  glUniform1f(loc, elapsed);
}

void ShaderProg::proj(const glm::mat4 &m) const noexcept {
  GLint loc = glGetUniformLocation(spid, "uProj");
  glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

} // namespace gfx
