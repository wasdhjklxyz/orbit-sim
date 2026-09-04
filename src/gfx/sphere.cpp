#include "geom.hpp"

#include <GLES3/gl3.h>
#include <glm/common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace gfx::geom {

/* NOTE: If sectors=2, stacks=2, it is a double sided square
         If sectors=18 stacks=2, it is a round disc with 2 sides */
namespace {
static constexpr GLuint MIN_SECTOR_COUNT = 2;
static constexpr GLuint MIN_STACK_COUNT = 2;
} // namespace

Sphere::Sphere(GLuint sectors, GLuint stacks) noexcept
    : sectors{glm::max(sectors, MIN_SECTOR_COUNT)},
      stacks{glm::max(stacks, MIN_STACK_COUNT)} {
  vertices_.reserve((stacks + 1) * (sectors + 1));
  indices_.reserve((stacks - 1) * sectors * 6);
  build_vertices();
  build_indices();
}

void Sphere::build_vertices() {
  const float stack_step = glm::pi<float>() / stacks;
  const float sector_step = glm::two_pi<float>() / sectors;
  vertices_.clear();
  for (GLuint i = 0; i <= stacks; i++) {
    const float phi = glm::half_pi<float>() - i * stack_step;
    for (GLuint k = 0; k <= sectors; k++) {
      const float theta = sector_step * k;
      vertices_.push_back(glm::vec3(glm::cos(phi) * glm::cos(theta),
                                    glm::cos(phi) * glm::sin(theta),
                                    glm::sin(phi)));
    }
  }
}

void Sphere::build_indices() {
  indices_.clear();
  for (GLuint i = 0; i < stacks; i++) {
    auto k1 = i * (sectors + 1); // NOTE: Beginning of current stack
    auto k2 = k1 + sectors + 1;  // NOTE: Beginning of next stack
    for (GLuint k = 0; k < sectors; k++, k1++, k2++) {
      /* NOTE: Two triangles per sector excluding first and last stacks */
      if (i != 0) {
        indices_.push_back(k1);
        indices_.push_back(k2);
        indices_.push_back(k1 + 1);
      }
      if (i != stacks - 1) {
        indices_.push_back(k1 + 1);
        indices_.push_back(k2);
        indices_.push_back(k2 + 1);
      }
    }
  }
}

} // namespace gfx::geom
