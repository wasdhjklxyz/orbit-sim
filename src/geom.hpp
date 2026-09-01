#pragma once

#include <GLES3/gl3.h>
#include <cstddef>
#include <glm/vec3.hpp>
#include <span>

#include "config.h"

namespace gfx::geom {

class Mesh {
protected:
  virtual void build_vertices();
  virtual void build_indicies();
  std::vector<glm::vec3> vertices_;
  std::vector<GLsizei> indicies_; // NOTE: CCW index list of triangles

public:
  std::span<const glm::vec3> vertices() const noexcept { return vertices_; }
  std::span<const GLsizei> indicies() const noexcept { return indicies_; }
};

class Sphere : Mesh {
public:
  explicit Sphere(float radius = 1, GLsizei sectors = NUM_SECTORS_SPHERE,
                  GLsizei stacks = NUM_STACKS_SPHERE) noexcept;

private:
  void build_vertices() override;
  void build_indicies() override;

  float radius;
  GLsizei sectors; // NOTE: Longitude, # of slices
  GLsizei stacks;  // NOTE: Latitude, # of stacks
};

class Circle : Mesh {
public:
  explicit Circle(float radius = 1, GLsizei sectors = NUM_SECTORS_CIRCLE);

private:
  void build_vertices() override;
  void build_indicies() override;

  float radius;
  GLsizei sectors;
};

} // namespace gfx::geom
