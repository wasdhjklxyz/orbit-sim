#pragma once

#include <expected>
#include <glm/vec3.hpp>
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
  void draw_sphere(const glm::vec4 &) noexcept;
  void present(double delta_time) noexcept;
  void move_camera(bool forward, bool backward, bool right, bool left, bool up,
                   bool down, float mouse_dx, float mouse_dy,
                   double delta_time) noexcept; /* FIXME LOL */

private:
  struct Impl;
  explicit Renderer(std::unique_ptr<Impl> p) noexcept;
  std::unique_ptr<Impl> impl;
};

}; // namespace gfx
