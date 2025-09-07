#pragma once
#include <GL/gl.h>
#include <array>
#include <cmath>
#include <format>

namespace Utils {
// Generic 3D vector template.
template <typename T> struct Vector3 {
public:
  Vector3() : m_x(0), m_y(0), m_z(0){};
  Vector3(T x_, T y_, T z_) : m_x(x_), m_y(y_), m_z(z_) {}
  Vector3(const std::array<T, 3> &arr)
      : m_x(arr[0]), m_y(arr[1]), m_z(arr[2]) {}

  const std::array<T, 3> &AsArray() const { return {m_x, m_y, m_z}; }

  inline T X() const { return m_x; }
  inline T Y() const { return m_y; }
  inline T Z() const { return m_z; }

  inline T &mut_X() { return m_x; }
  inline T &mut_Y() { return m_y; }
  inline T &mut_Z() { return m_z; }

  inline void Set(T x_, T y_, T z_) {
    m_x = x_;
    m_y = y_;
    m_z = z_;
  }

  inline void Clear() {
    m_x = 0;
    m_y = 0;
    m_z = 0;
  }

  inline T LengthSquared() const { return m_x * m_x + m_y * m_y + m_z * m_z; }
  inline T Length() const { return std::sqrt(LengthSquared()); }

  inline T Dot(const Vector3 &other) const {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
  }

  Vector3 Cross(const Vector3 &other) const {
    return Vector3(m_y * other.m_z - m_z * other.m_y,
                   m_z * other.m_x - m_x * other.m_z,
                   m_x * other.m_y - m_y * other.m_x);
  }

  // Returns normalized vector.
  Vector3 Normalized() const {
    T len = Length();
    if (len == 0)
      return Vector3(0, 0, 0); // Avoid division by zero
    return Vector3(m_x / len, m_y / len, m_z / len);
  }

  Vector3 operator+(const Vector3 &other) const {
    return Vector3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
  }

  Vector3 operator-(const Vector3 &other) const {
    return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
  }

  Vector3 operator*(T scalar) const {
    return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);
  }

  Vector3 operator/(T scalar) const {
    return Vector3(m_x / scalar, m_y / scalar, m_z / scalar);
  }

  Vector3 &operator+=(const Vector3 &other) {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
  }

  Vector3 &operator-=(const Vector3 &other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
    return *this;
  }

  bool operator==(const Vector3 &other) const {
    return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
  }

  bool operator!=(const Vector3 &other) const { return !(*this == other); }

  T Dist(const Vector3 &other) const {
    T dx = m_x - other.m_x;
    T dy = m_y - other.m_y;
    T dz = m_z - other.m_z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  Vector3 &operator=(const std::array<T, 3> &arr) {
    m_x = arr[0];
    m_y = arr[1];
    m_z = arr[2];
    return *this;
  }

  template <size_t N> Vector3 &operator=(const T (&arr)[N]) {
    static_assert(N == 3, "Array must have exactly 3 elements.");
    m_x = arr[0];
    m_y = arr[1];
    m_z = arr[2];
    return *this;
  }

private:
  T m_x, m_y, m_z;
};

} // namespace Utils

// std::formatter specialization for Utils::Vector3.
template <typename T> struct std::formatter<Utils::Vector3<T>> {
  constexpr auto parse(std::format_parse_context &ctx) const {
    return ctx.begin();
  }

  // Formats vector as "(x, y, z)".
  template <typename FormatContext>
  auto format(const Utils::Vector3<T> &vec, FormatContext &ctx) const {
    return std::format_to(ctx.out(), "({}, {}, {})", vec.X(), vec.Y(), vec.Z());
  }
};