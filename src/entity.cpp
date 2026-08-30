#include "entity.hpp"

#include <cmath>
#include <ctime>

#include "vec.hpp"

Entity::Entity(Vec3d pos, Vec3d vel, double radius) noexcept
    : pos{pos}, vel{vel}, radius{radius} {}

void Entity::tick(const float deltaTime) noexcept { pos += vel * deltaTime; }

void Entity::collide(Entity &other) noexcept {
  const auto delta = other.pos - pos;
  const auto r = radius + other.radius;
  const auto d2 = delta.length_squared();
  if (d2 > r * r)
    return;

  const auto d = std::sqrt(d2);
  const auto depth = r - d;
  const auto normal = delta / d;
  pos += -1 * normal * depth / 2;
  other.pos += normal * depth / 2;

  const auto old_vel = vel;
  vel = ((radius - other.radius) / (radius + other.radius) * vel) +
        ((2 * other.radius) / (radius + other.radius) * other.vel);
  other.vel = ((2 * radius) / (radius + other.radius) * old_vel) +
              ((other.radius - radius) / (radius + other.radius) * other.vel);
}
