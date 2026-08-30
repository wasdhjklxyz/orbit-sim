#pragma once

#include <span>
#include <vector>

#include "entity.hpp"

class Sim {
public:
  Sim(double width, double height) noexcept;
  void add(Entity);
  std::span<const Entity> get() const noexcept;
  void tick(const float deltaTime) noexcept;

private:
  double width, height;
  std::vector<Entity> entities;
};
