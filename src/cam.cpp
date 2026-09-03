#include "gfx_internal.hpp"

#include "config.h"

namespace gfx {

namespace {

inline constexpr float ACCELERATION = 60.f;
inline constexpr float MAX_VELOCITY = 20.f;
inline constexpr float DRAG = 8.f;
inline constexpr float EPS = 1e-4f; // NOTE: Threshold for drag. World units/s
inline constexpr float EPS2 = EPS * EPS;
inline constexpr float PITCH_CLAMP_DEG = 89.f;

glm::vec3 dir_euler_angles(float pitch_rad, float yaw_rad) noexcept {
  return glm::normalize(glm::vec3{glm::cos(yaw_rad) * glm::cos(pitch_rad),
                                  glm::sin(pitch_rad),
                                  glm::sin(yaw_rad) * glm::cos(pitch_rad)});
}

} // namespace

Camera::Camera(glm::vec3 pos, float pitch_deg, float yaw_deg) noexcept
    : pos{pos}, vel{0.f},
      dir{dir_euler_angles(glm::radians(pitch_deg), glm::radians(yaw_deg))},
      right_{glm::normalize(glm::cross(dir, WORLD_UP))},
      up{glm::cross(right_, dir)}, pitch_deg{pitch_deg}, yaw_deg{yaw_deg} {}

glm::mat4 Camera::view() const noexcept {
  return glm::lookAt(pos, pos + dir, up);
}

void Camera::update(const glm::vec3 &acc_dir, float dpitch_deg, float dyaw_deg,
                    double delta_time) noexcept {
  look(dpitch_deg, dyaw_deg);
  move(acc_dir, delta_time);
};

void Camera::look(float dpitch_deg, float dyaw_deg) noexcept {
  yaw_deg += dyaw_deg * MOUSE_SENS;
  pitch_deg = glm::clamp(pitch_deg - dpitch_deg * MOUSE_SENS, -PITCH_CLAMP_DEG,
                         PITCH_CLAMP_DEG);
  dir = dir_euler_angles(glm::radians(pitch_deg), glm::radians(yaw_deg));
  right_ = glm::normalize(glm::cross(dir, WORLD_UP));
  up = glm::cross(right_, dir);
}

void Camera::move(const glm::vec3 &acc_dir, double delta_time) noexcept {
  if (glm::length2(acc_dir) > 0.f) {
    vel += glm::normalize(acc_dir) * ACCELERATION * (float)delta_time;
    const auto speed = glm::length(vel);
    if (speed > MAX_VELOCITY)
      vel *= MAX_VELOCITY / speed;
  } else {
    vel *= std::exp(-DRAG * delta_time);
    if (glm::length2(vel) < EPS2)
      vel = glm::vec3{0.f};
  }
  pos += vel * (float)delta_time;
}

} // namespace gfx
