#pragma once

#include <expected>
#include <memory>
#include <string>

#include "vec.hpp"

namespace gfx {

class Renderer {
public:
  ~Renderer();
  Renderer(Renderer &&) noexcept;
  Renderer &operator=(Renderer &&) noexcept;

  static std::expected<Renderer, std::string> create();
  void clear() noexcept;
  void draw(const float deltaTime, const Vec3d &pos) noexcept;

private:
  struct Impl;
  explicit Renderer(std::unique_ptr<Impl> p) noexcept;
  std::unique_ptr<Impl> impl;
};

}; // namespace gfx
