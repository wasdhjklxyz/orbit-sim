#include "sim.hpp"

void Sim::add(Entity &&e) { entities.push_back(e); }

std::span<const Entity> Sim::get() const noexcept { return entities; }

void Sim::tick(const float deltaTime) noexcept {
  for (auto &e : entities)
    e.tick(deltaTime);
}
