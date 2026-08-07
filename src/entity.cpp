#include "entity.hpp"

Entity::Entity(Vec3d pos, Vec3d vel) noexcept : pos{pos}, vel{vel} {}

void Entity::tick(const float deltaTime) noexcept { pos += vel * deltaTime; }
