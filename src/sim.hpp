#pragma once

#include <span>
#include <vector>

#include "entity.hpp"

class Sim {
public:
  void add(Entity);
  std::span<const Entity> get() const noexcept;
  void tick(const float deltaTime) noexcept;

private:
  std::vector<Entity> entities;
};
