#pragma once

#include <glm/ext/vector_double2.hpp>

struct Entity {
  explicit Entity(glm::dvec2 pos, glm::dvec2 vel, double radius) noexcept;

  void tick(const float deltaTime) noexcept;
  void tick(const Entity &other) noexcept;
  bool intersects(const Entity &other) const noexcept;
  void collide(Entity &other) noexcept;

  glm::dvec2 pos;
  glm::dvec2 vel;
  double radius;
};

struct Wall { // NOTE: Infinite mass
  explicit Wall(glm::dvec2 a, glm::dvec2 b) noexcept;
  glm::dvec2 a, b;
};
