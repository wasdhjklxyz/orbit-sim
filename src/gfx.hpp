#pragma once

#include <expected>
#include <glm/vec4.hpp>
#include <memory>
#include <string>

namespace gfx {

class Renderer {
public:
  ~Renderer();
  Renderer(Renderer &&) noexcept;
  Renderer &operator=(Renderer &&) noexcept;

  static std::expected<Renderer, std::string> create();
  void clear() noexcept;
  void draw_circle(const glm::vec4 &) noexcept;
  void present(const float deltaTime) noexcept;

private:
  struct Impl;
  explicit Renderer(std::unique_ptr<Impl> p) noexcept;
  std::unique_ptr<Impl> impl;
};

}; // namespace gfx
