#pragma once

#include <glm/ext/vector_double3.hpp>

struct Entity {
  explicit Entity(glm::dvec3 pos, glm::dvec3 vel, double radius,
                  double mass) noexcept;

  void tick(const float deltaTime) noexcept;

  glm::dvec3 pos;
  glm::dvec3 vel;
  glm::dvec3 acc;
  double radius;
  double mass;
};
