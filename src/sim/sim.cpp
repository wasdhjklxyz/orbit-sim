#define GLM_ENABLE_EXPERIMENTAL

#include <gsb/config.h>
#include <gsb/sim.hpp>

#include <glm/exponential.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include <cstddef>
#include <span>

void Sim::add(Entity e) { entities.push_back(e); }

std::span<const Entity> Sim::get() const noexcept { return entities; }

void Sim::apply_gravity(Entity &e1, Entity &e2) noexcept {
  const auto mu = G * e2.mass;
  const auto d = e1.pos - e2.pos;
  const auto r2 = glm::length2(d);
  e1.acc += (-mu / r2) * glm::normalize(d);
}

void Sim::tick(double delta_time) noexcept {
  for (auto &e : entities)
    e.acc = {0, 0, 0};

  for (std::size_t i = 0; i < entities.size(); i++)
    for (std::size_t k = 0; k < entities.size(); k++) {
      if (k == i)
        continue;
      apply_gravity(entities[i], entities[k]);
    }

  for (auto &e : entities)
    e.tick(delta_time);
}

Entity::Entity(glm::dvec3 pos, glm::dvec3 vel, double radius,
               double mass) noexcept
    : pos{pos}, vel{vel}, acc{0}, radius{radius}, mass{mass} {}

void Entity::tick(double delta_time) noexcept {
  vel += acc * delta_time;
  pos += vel * delta_time;
}
