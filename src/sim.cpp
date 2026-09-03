#include "sim.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include <ctime>
#include <glm/exponential.hpp>
#include <glm/ext/vector_double3.hpp>

void Sim::add(Entity e) { entities.push_back(e); }

std::span<const Entity> Sim::get() const noexcept { return entities; }

void Sim::apply_gravity(Entity &e1, Entity &e2, float deltaTime) noexcept {
  /* FIXME: deltatime should be double not float */
  constexpr auto G = 100; // 6.6743e-11;
  const auto mu = G * e2.mass;
  const auto d = e1.pos - e2.pos;
  const auto r2 = glm::length2(d);
  e1.acc += (-mu / r2) * glm::normalize(d) * (double)deltaTime;
}

void Sim::tick(const float deltaTime) noexcept {
  for (std::size_t i = 0; i < entities.size(); i++)
    for (std::size_t k = 0; k < entities.size(); k++) {
      if (k == i)
        continue;
      apply_gravity(entities[i], entities[k], deltaTime);
    }

  for (auto &e : entities)
    e.tick(deltaTime);
}

Entity::Entity(glm::dvec3 pos, glm::dvec3 vel, double radius,
               double mass) noexcept
    : pos{pos}, vel{vel}, acc{0}, radius{radius}, mass{mass} {}

void Entity::tick(const float deltaTime) noexcept {
  vel += acc * double(deltaTime);
  pos += vel * double(deltaTime);
}
