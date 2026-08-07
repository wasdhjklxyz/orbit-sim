#pragma once

#include <concepts>

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

  constexpr Vec3 &operator-=(const Vec3 &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  friend constexpr Vec3 operator-(Vec3 lhs, const Vec3 &rhs) noexcept {
    lhs -= rhs;
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

  constexpr Vec3 &operator/=(T s) noexcept {
    const T inv = T{1} / s;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
  }

  friend constexpr Vec3 operator/(Vec3 lhs, T s) noexcept {
    lhs /= s;
    return lhs;
  }

  friend constexpr Vec3 operator/(T s, Vec3 rhs) noexcept {
    rhs /= s;
    return rhs;
  }

  template <typename U>
    requires(!std::same_as<U, T>)
  explicit constexpr operator Vec3<U>() const noexcept {
    return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
  }

  T x, y, z;
};

using Vec3d = Vec3<double>;
using Vec3f = Vec3<float>;

template <typename T>
constexpr T dot(const Vec3<T> &a, const Vec3<T> &b) noexcept {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T> constexpr T length_squared(const Vec3<T> &v) noexcept {
  return dot(v, v);
}
