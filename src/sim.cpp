#include "sim.hpp"

#include "config.h"

void Sim::add(Entity e) { entities.push_back(e); }

std::span<const Entity> Sim::get() const noexcept { return entities; }

void Sim::tick(const float deltaTime) noexcept {
  for (auto &e : entities)
    e.tick(deltaTime);

  // NOTE: Yeah ik this is bummy asl its 1 am tired rn just wanna see balls
  // bounce (hahhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh)
  const float hw = METERS_WIDTH / 2.0f;
  const float hh = METERS_HEIGHT / 2.0f;
  for (auto &e : entities) {
    if (e.pos.x + e.radius > hw) {
      e.pos.x = hw - e.radius;
      e.vel.x *= -1;
    } else if (e.pos.x - e.radius < -hw) {
      e.pos.x = -hw + e.radius;
      e.vel.x *= -1;
    }
    if (e.pos.y + e.radius > hh) {
      e.pos.y = hh - e.radius;
      e.vel.y *= -1;
    } else if (e.pos.y - e.radius < -hh) {
      e.pos.y = -hh + e.radius;
      e.vel.y *= -1;
    }
  }

  // TODO: Do NOT move any postions velocities and shit should keep note of
  // everything and then average it out before changing thigns
  for (std::size_t i = 0; i < entities.size(); i++)
    for (std::size_t k = i + 1; k < entities.size(); k++)
      entities[i].collide(entities[k]); // NOTE: Early return if no collision
}
