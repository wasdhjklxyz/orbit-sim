#pragma once

#include <span>
#include <vector>

#include "entity.hpp"

class Sim {
public:
  void add(Entity);
  std::span<const Entity> get() const noexcept;
  void tick(const float deltaTime) noexcept;
  double energy() const noexcept;

private:
  void apply_gravity(Entity &e1, Entity &e2) noexcept;
  std::vector<Entity> entities;
};
