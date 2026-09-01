#pragma once

#include <cstddef>
#include <glm/vec3.hpp>

#include "config.h"

namespace gfx::geom {

class Mesh {
protected:
  virtual void build_vertices();
  virtual void build_indicies();
  std::vector<glm::vec3> vertices;
  std::vector<std::size_t> indicies; // NOTE: CCW index list of triangles
};

class Sphere : Mesh {
public:
  explicit Sphere(float radius = 1, std::size_t sectors = NUM_SECTORS_SPHERE,
                  std::size_t stacks = NUM_STACKS_SPHERE) noexcept;

private:
  void build_vertices() override;
  void build_indicies() override;

  float radius;
  std::size_t sectors; // NOTE: Longitude, # of slices
  std::size_t stacks;  // NOTE: Latitude, # of stacks
};

class Circle : Mesh {
public:
  explicit Circle(float radius = 1, std::size_t sectors = NUM_SECTORS_CIRCLE);

private:
  void build_vertices() override;
  void build_indicies() override;

  float radius;
  std::size_t sectors;
};

} // namespace gfx::geom
