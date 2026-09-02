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
static constexpr GLuint MIN_SECTOR_COUNT = 1;
} // namespace

Circle::Circle(GLuint sectors) : sectors{glm::max(sectors, MIN_SECTOR_COUNT)} {
  vertices_.reserve(sectors + 1);
  indicies_.reserve(sectors * 3);
  build_vertices();
  build_indicies();
}

void Circle::build_vertices() {
  const float sector_step = glm::two_pi<float>() / sectors;
  vertices_.clear();
  vertices_.push_back({0, 0, 0}); // NOTE: Center
  for (GLuint k = 0; k < sectors; k++) {
    const float theta = sector_step * k;
    vertices_.push_back(glm::vec3(glm::cos(theta), glm::sin(theta), 0));
  }
}

void Circle::build_indicies() {
  indicies_.clear();
  constexpr auto center_idx = 0;
  for (GLuint i = 0; i < sectors; i++) {
    indicies_.push_back(i + 1);
    indicies_.push_back(1 + ((i + 1) % sectors));
    indicies_.push_back(center_idx);
  }
}

} // namespace gfx::geom
