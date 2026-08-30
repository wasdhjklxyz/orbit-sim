#include "sim.hpp"

void Sim::add(Entity e) { entities.push_back(e); }

std::span<const Entity> Sim::get() const noexcept { return entities; }

void Sim::tick(const float deltaTime) noexcept {
  for (auto &e : entities)
    e.tick(deltaTime);

  for (std::size_t i = 0; i < entities.size(); i++)
    for (std::size_t k = i + 1; k < entities.size(); k++)
      entities[i].collide(entities[k]); // NOTE: Early return if no collision
}
