#include "entity.hpp"

#include <ctime>
#include <glm/common.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/geometric.hpp>

Entity::Entity(glm::dvec3 pos, glm::dvec3 vel, double radius) noexcept
    : pos{pos}, vel{vel}, radius{radius} {}

void Entity::tick(const float deltaTime) noexcept {
  pos += vel * double(deltaTime);
}

void Entity::collide(Entity &other) noexcept {
  const auto delta = other.pos - pos;
  const auto r = radius + other.radius;
  const auto d2 = glm::length(delta) * glm::length(delta); // FIXME
  if (d2 > r * r)
    return;

  const auto d = glm::sqrt(d2);
  const auto depth = r - d;
  const auto normal = delta / d;
  pos += double(-1) * normal * depth / double(2);
  other.pos += normal * depth / double(2);

  const auto old_vel = vel;
  vel = ((radius - other.radius) / (radius + other.radius) * vel) +
        ((2 * other.radius) / (radius + other.radius) * other.vel);
  other.vel = ((2 * radius) / (radius + other.radius) * old_vel) +
              ((other.radius - radius) / (radius + other.radius) * other.vel);
}
