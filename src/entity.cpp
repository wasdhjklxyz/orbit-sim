#include "entity.hpp"

#include <ctime>
#include <glm/common.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/geometric.hpp>

Entity::Entity(glm::dvec3 pos, glm::dvec3 vel, double radius,
               double mass) noexcept
    : pos{pos}, vel{vel}, acc{0}, radius{radius}, mass{mass} {}

void Entity::tick(const float deltaTime) noexcept {
  vel += acc * double(deltaTime);
  pos += vel * double(deltaTime);
}
