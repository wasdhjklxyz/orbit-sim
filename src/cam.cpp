#include "gfx_internal.hpp"

namespace gfx {

namespace {

inline constexpr float ACCELERATION = 60.f;
inline constexpr float MAX_VELOCITY = 20.f;
inline constexpr float DRAG = 8.f;

inline constexpr glm::vec3 dir_euler_angles(float pitch, float yaw) noexcept {
  return {glm::cos(glm::radians(yaw)), glm::cos(glm::radians(pitch)),
          glm::sin(glm::radians(yaw))};
}

} // namespace

Camera::Camera(glm::vec3 pos, float pitch, float yaw) noexcept
    : pos{pos}, dir{dir_euler_angles(pitch, yaw)},
      right_{glm::normalize(glm::cross(dir, WORLD_UP))},
      up{glm::cross(right_, dir)} {}

void Camera::move(const glm::vec3 &move_dir, float pitch, float yaw) noexcept {
  dir = glm::normalize(move_dir + dir_euler_angles(pitch, yaw));
}

void Camera::update(double delta_time) noexcept {
  const auto drag = glm::exp(-DRAG * (float)delta_time);
  vel += (dir * ACCELERATION + drag) * (float)delta_time;

  const auto speed = glm::length(vel);
  if (speed > MAX_VELOCITY)
    vel *= MAX_VELOCITY / speed;

  right_ = glm::normalize(glm::cross(dir, WORLD_UP));
  up = glm::cross(right_, dir);
}

} // namespace gfx
