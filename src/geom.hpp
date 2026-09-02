#pragma once

#include <GLES3/gl3.h>
#include <cstddef>
#include <glm/vec3.hpp>
#include <span>

#include "config.h"

namespace gfx::geom {

class Mesh {
protected:
  ~Mesh() = default;
  std::vector<glm::vec3> vertices_;
  std::vector<GLuint> indicies_; // NOTE: CCW index list of triangles

public:
  std::span<const glm::vec3> vertices() const noexcept { return vertices_; }
  std::span<const GLuint> indicies() const noexcept { return indicies_; }
};

class Sphere : public Mesh {
public:
  explicit Sphere(GLuint sectors = NUM_SECTORS_SPHERE,
                  GLuint stacks = NUM_STACKS_SPHERE) noexcept;

private:
  void build_vertices();
  void build_indicies();

  GLuint sectors; // NOTE: Longitude, # of slices
  GLuint stacks;  // NOTE: Latitude, # of stacks
};

class Circle : public Mesh {
public:
  explicit Circle(GLuint sectors = NUM_SECTORS_CIRCLE);

private:
  void build_vertices();
  void build_indicies();

  GLuint sectors;
};

} // namespace gfx::geom
