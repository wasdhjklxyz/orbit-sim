#pragma once

template <typename T> struct Vec3 {
  constexpr Vec3 &operator+=(const Vec3 &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  friend constexpr Vec3 operator+(Vec3 lhs, const Vec3 &rhs) noexcept {
    lhs += rhs;
    return lhs;
  }

  constexpr Vec3 &operator*=(T s) noexcept {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  friend constexpr Vec3 operator*(Vec3 lhs, T s) noexcept {
    lhs *= s;
    return lhs;
  }

  friend constexpr Vec3 operator*(T s, Vec3 rhs) noexcept {
    rhs *= s;
    return rhs;
  }

  T x, y, z;
};

using Vec3d = Vec3<double>;
