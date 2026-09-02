#include "sim.hpp"

#include <glm/exponential.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "config.h"
#include "entity.hpp"

namespace {
constexpr auto EPSILON = 5.0;
}; // namespace

void Sim::add(Entity e) { entities.push_back(e); }

std::span<const Entity> Sim::get() const noexcept { return entities; }

void Sim::apply_gravity(Entity &e1, Entity &e2) noexcept {
  const auto d = e2.pos - e1.pos;
  const auto r2 = glm::length2(d) + EPSILON * EPSILON;
  const auto inv_r = 1.0 / glm::sqrt(r2);
  const auto f = G * e1.mass * e2.mass * inv_r * inv_r; // magnitude
  const auto dir = d * inv_r;                           // normalized
  e1.acc += (f / e1.mass) * dir;
  e2.acc -= (f / e2.mass) * dir;
}

void Sim::tick(const float deltaTime) noexcept {
  for (auto &e : entities)
    e.acc = {0, 0, 0};

  for (std::size_t i = 0; i < entities.size(); i++)
    for (std::size_t k = i + 1; k < entities.size(); k++)
      apply_gravity(entities[i], entities[k]);

  for (auto &e : entities)
    e.tick(deltaTime);
}

double Sim::energy() const noexcept {
  double ke = 0, pe = 0;
  for (const auto &e : entities)
    ke += 0.5 * e.mass * glm::length2(e.vel);
  for (size_t i = 0; i < entities.size(); i++)
    for (size_t k = i + 1; k < entities.size(); k++)
      pe -= G * entities[i].mass * entities[k].mass /
            glm::sqrt(glm::length2(entities[k].pos - entities[i].pos) +
                      EPSILON * EPSILON);
  return ke + pe;
}
