#include "entity.hpp"

Entity::Entity(Vec3d pos, Vec3d vel, double radius) noexcept
    : pos{pos}, vel{vel}, radius{radius} {}

void Entity::tick(const float deltaTime) noexcept { pos += vel * deltaTime; }
