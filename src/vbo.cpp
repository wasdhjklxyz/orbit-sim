#include "gfx_internal.hpp"

#include <GLES3/gl3.h>
#include <cmath>
#include <numbers>
#include <vector>

namespace gfx {

namespace {

constexpr double METERS_PER_NDC = 200;
constexpr double INV_METERS_PER_NDC = 1.0 / METERS_PER_NDC;

std::vector<Vec3f> unit_circle(std::size_t n) {
  constexpr float TAU = 2.0 * std::numbers::pi_v<double>;
  std::vector<Vec3f> uc(n);
  for (std::size_t i = 0; i < n; i++) {
    const float t = TAU * i / n;
    uc[i] = Vec3f{std::cos(t), std::sin(t), 0.0};
  }
  return uc;
}

static const auto UNIT_CIRCLE = unit_circle(NUM_VERTICES_CIRCLE - 2);

} // namespace

VertexBuffer::VertexBuffer(GLuint id) noexcept : id{id} {}
VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : id{std::exchange(other.id, 0)} {}

VertexBuffer::~VertexBuffer() noexcept {
  // TODO: Unbind?
  glDeleteBuffers(1, &id);
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &id);
    id = std::exchange(other.id, 0);
  }
  return *this;
}

VertexBuffer VertexBuffer::create() {
  // TODO: Error handling/checking, also fix hardcoded values lol
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES_CIRCLE * sizeof(GLfloat) * 3,
               nullptr, GL_DYNAMIC_DRAW);

  // NOTE: Position attrs
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);
  // NOTE: Hue attrs
  // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE,
  //                       NUM_VERTICES_CIRCLE * sizeof(GLfloat),
  //                       (void *)(3 * sizeof(GLfloat)));
  // glEnableVertexAttribArray(1);

  return VertexBuffer{id};
}

void VertexBuffer::bind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, id); }
void VertexBuffer::unbind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// FIXME: THIS FUCKING SUCKS
void VertexBuffer::update(const Vec3d &pos, const double radius) noexcept {
  Vec3f posf = static_cast<Vec3f>(pos * INV_METERS_PER_NDC);

  std::array<Vec3f, NUM_VERTICES_CIRCLE> verts{};
  verts[0] = posf;
  for (std::size_t i = 0; i < NUM_VERTICES_CIRCLE - 2; i++)
    verts[i + 1] = posf + (UNIT_CIRCLE[i] * radius * INV_METERS_PER_NDC);
  verts[NUM_VERTICES_CIRCLE - 1] = verts[1];

  // FIXME: Width height are defined in app.cpp!!!!!! Refactor!!!!!!!!!!
  constexpr float aspect = float(720) / float(1280);
  for (std::size_t i = 0; i < verts.size(); i++)
    verts[i].x *= aspect;

  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verts.size() * 3,
               verts.data(), GL_DYNAMIC_DRAW);
}

} // namespace gfx
