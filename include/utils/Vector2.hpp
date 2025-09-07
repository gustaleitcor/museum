#pragma once
#include <GL/gl.h>
#include <array>
#include <cmath>
#include <format>

namespace Utils {
// Generic 2D vector template.
template <typename T> struct Vector2 {
public:
  Vector2() : m_x(0), m_y(0) {};
  Vector2(T x_, T y_) : m_x(x_), m_y(y_) {}
  Vector2(const std::array<T, 2> &arr)
      : m_x(arr[0]), m_y(arr[1]) {}

  const std::array<T, 2> AsArray() const { return {m_x, m_y}; }

  inline T X() const { return m_x; }
  inline T Y() const { return m_y; }

  inline T &mut_X() { return m_x; }
  inline T &mut_Y() { return m_y; }

  inline void Set(T x_, T y_) {
    m_x = x_;
    m_y = y_;
  }

  inline void Clear() {
    m_x = 0;
    m_y = 0;
  }

  inline T LengthSquared() const { return m_x * m_x + m_y * m_y; }
  inline T Length() const { return std::sqrt(LengthSquared()); }

  inline T Dot(const Vector2 &other) const {
    return m_x * other.m_x + m_y * other.m_y;
  }

  T Cross(const Vector2 &other) const {
    return m_x * other.m_y - m_y * other.m_x;
  }

  // Returns normalized vector.
  Vector2 Normalized() const {
    T len = Length();
    if (len == 0)
      return Vector2(0, 0); // Avoid division by zero
    return Vector2(m_x / len, m_y / len);
  }

  Vector2 operator+(const Vector2 &other) const {
    return Vector2(m_x + other.m_x, m_y + other.m_y);
  }

  Vector2 operator-(const Vector2 &other) const {
    return Vector2(m_x - other.m_x, m_y - other.m_y);
  }

  Vector2 operator*(T scalar) const {
    return Vector2(m_x * scalar, m_y * scalar);
  }

  Vector2 operator/(T scalar) const {
    return Vector2(m_x / scalar, m_y / scalar);
  }

  Vector2 &operator+=(const Vector2 &other) {
    m_x += other.m_x;
    m_y += other.m_y;
    return *this;
  }

  Vector2 &operator-=(const Vector2 &other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    return *this;
  }

  bool operator==(const Vector2 &other) const {
    return m_x == other.m_x && m_y == other.m_y;
  }

  bool operator!=(const Vector2 &other) const { return !(*this == other); }

  T Dist(const Vector2 &other) const {
    T dx = m_x - other.m_x;
    T dy = m_y - other.m_y;
    return std::sqrt(dx * dx + dy * dy);
  }

  Vector2 &operator=(const std::array<T, 2> &arr) {
    m_x = arr[0];
    m_y = arr[1];
    return *this;
  }

  template <size_t N> Vector2 &operator=(const T (&arr)[N]) {
    static_assert(N == 2, "Array must have exactly 2 elements.");
    m_x = arr[0];
    m_y = arr[1];
    return *this;
  }

private:
  T m_x, m_y;
};

} // namespace Utils

// std::formatter specialization for Utils::Vector2.
template <typename T> struct std::formatter<Utils::Vector2<T>> {
  constexpr auto parse(std::format_parse_context &ctx) const {
    return ctx.begin();
  }

  // Formats vector as "(x, y)".
  template <typename FormatContext>
  auto format(const Utils::Vector2<T> &vec, FormatContext &ctx) const {
    return std::format_to(ctx.out(), "({}, {})", vec.X(), vec.Y());
  }
};