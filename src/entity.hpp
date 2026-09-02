#pragma once

#include <glm/ext/vector_double3.hpp>

struct Entity {
  explicit Entity(glm::dvec3 pos, glm::dvec3 vel, double radius) noexcept;

  void tick(const float deltaTime) noexcept;
  void tick(const Entity &other) noexcept;
  bool intersects(const Entity &other) const noexcept;
  void collide(Entity &other) noexcept;

  glm::dvec3 pos;
  glm::dvec3 vel;
  double radius;
};

struct Wall { // NOTE: Infinite mass
  explicit Wall(glm::dvec3 a, glm::dvec3 b) noexcept;
  glm::dvec3 a, b;
};
