#pragma once

#include <gsb/config.h>

#include <GLES3/gl3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>

#include <cstddef>
#include <span>
#include <vector>

namespace gfx::geom {

class Mesh {
protected:
  ~Mesh() = default;
  std::vector<glm::vec3> vertices_;
  std::vector<GLuint> indices_; // NOTE: CCW index list of triangles

public:
  std::span<const glm::vec3> vertices() const noexcept { return vertices_; }
  std::span<const GLuint> indices() const noexcept { return indices_; }
};

class Sphere : public Mesh {
public:
  explicit Sphere(GLuint sectors = NUM_SECTORS_SPHERE,
                  GLuint stacks = NUM_STACKS_SPHERE) noexcept;

private:
  void build_vertices();
  void build_indices();

  GLuint sectors; // NOTE: Longitude, # of slices
  GLuint stacks;  // NOTE: Latitude, # of stacks
};

class Circle : public Mesh {
public:
  explicit Circle(GLuint sectors = NUM_SECTORS_CIRCLE);

private:
  void build_vertices();
  void build_indices();

  GLuint sectors;
};

} // namespace gfx::geom
