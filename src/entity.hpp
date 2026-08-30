#pragma once

#include "vec.hpp"

struct Entity {
  explicit Entity(Vec3d pos, Vec3d vel, double radius) noexcept;

  void tick(const float deltaTime) noexcept;
  void tick(const Entity &other) noexcept;
  bool intersects(const Entity &other) const noexcept;
  void collide(Entity &other) noexcept;

  Vec3d pos;
  Vec3d vel;
  double radius;
};

struct Wall { // NOTE: Infinite mass
  explicit Wall(Vec3d a, Vec3d b) noexcept;
  Vec3d a, b;
};
