#pragma once

#include <glm/ext/vector_double3.hpp>
#include <span>
#include <vector>

struct Entity {
  explicit Entity(glm::dvec3 pos, glm::dvec3 vel, double radius,
                  double mass) noexcept;

  void tick(double delta_time) noexcept;

  glm::dvec3 pos;
  glm::dvec3 vel;
  glm::dvec3 acc;
  double radius;
  double mass;
};

class Sim {
public:
  void add(Entity);
  std::span<const Entity> get() const noexcept;
  void tick(double delta_time) noexcept;

private:
  void apply_gravity(Entity &e1, Entity &e2, double delta_time) noexcept;
  std::vector<Entity> entities;
};
