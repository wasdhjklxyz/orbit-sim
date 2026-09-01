#include "geom.hpp"

#include <cstddef>
#include <glm/common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace gfx::geom {

/* NOTE: If sectors=1 then its just a triangle */
namespace {
static constexpr std::size_t MIN_SECTOR_COUNT = 1;
} // namespace

Circle::Circle(float radius, std::size_t sectors)
    : radius{glm::max(radius, float(0))},
      sectors{glm::max(sectors, MIN_SECTOR_COUNT)} {
  vertices.reserve(sectors + 1);
  indicies.reserve(sectors * 3);
  build_vertices();
  build_indicies();
}

void Circle::build_vertices() {
  const float sector_step = glm::two_pi<float>() / sectors;
  vertices.clear();
  vertices.push_back({0, 0, 0}); // NOTE: Center
  for (std::size_t k = 0; k <= sectors; k++) {
    const float theta = sector_step * k;
    vertices.push_back(
        glm::vec3(glm::cos(theta) * radius, glm::sin(theta) * radius, 0));
  }
}

void Circle::build_indicies() {
  indicies.clear();
  constexpr auto center_idx = 0;
  for (std::size_t i = 0; i < sectors; i++) {
    indicies.push_back(i + 1);
    indicies.push_back((i + 2) % sectors);
    indicies.push_back(center_idx);
  }
}

} // namespace gfx::geom
