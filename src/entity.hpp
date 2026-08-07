#pragma once

#include "vec.hpp"

class Entity {
public:
  explicit Entity(Vec3d pos, Vec3d vel) noexcept;
  void tick(const float deltaTime) noexcept;

private:
  Vec3d pos;
  Vec3d vel;
};
