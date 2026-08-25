#pragma once

#include "vec.hpp"

struct Entity {
  explicit Entity(Vec3d pos, Vec3d vel, double radius) noexcept;
  void tick(const float deltaTime) noexcept;
  Vec3d pos;
  Vec3d vel;
  double radius;
};
