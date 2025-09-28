#pragma once
#include <GL/gl.h>
#include <array>
#include <cmath>
#include <format>

namespace Utils {
// Generic 3D vector template.
template <typename T> struct EulerAngle {
public:
  EulerAngle() : m_row(0), m_pitch(0), m_yaw(0){};
  EulerAngle(T row_, T pitch_, T yaw_) : m_row(row_), m_pitch(pitch_), m_yaw(yaw_) {}
  EulerAngle(const std::array<T, 3> &arr)
      : m_row(arr[0]), m_pitch(arr[1]), m_yaw(arr[2]) {}

  const std::array<T, 3> AsArray() const { return {m_row, m_pitch, m_yaw}; }

  inline T Row() const { return m_row; }
  inline T Pitch() const { return m_pitch; }
  inline T Yaw() const { return m_yaw; }

  inline T &mut_Row() { return m_row; }
  inline T &mut_Pitch() { return m_pitch; }
  inline T &mut_Yaw() { return m_yaw; }

  inline void Set(T row_, T pitch_, T yaw_) {
    m_row = row_;
    m_pitch = pitch_;
    m_yaw = yaw_;
  }

  inline void Clear() {
    m_row = 0;
    m_pitch = 0;
    m_yaw = 0;
  }

  inline T LengthSquared() const { return m_row * m_row + m_pitch * m_pitch + m_yaw * m_yaw; }
  inline T Length() const { return std::sqrt(LengthSquared()); }

  inline T Dot(const EulerAngle &other) const {
    return m_row * other.m_row + m_pitch * other.m_pitch + m_yaw * other.m_yaw;
  }

  EulerAngle Cross(const EulerAngle &other) const {
    return EulerAngle(m_pitch * other.m_yaw - m_yaw * other.m_pitch,
                   m_yaw * other.m_row - m_row * other.m_yaw,
                   m_row * other.m_pitch - m_pitch * other.m_row);
  }

  // Returns normalized vector.
  EulerAngle Normalized() const {
    T len = Length();
    if (len == 0)
      return EulerAngle(0, 0, 0); // Avoid division by zero
    return EulerAngle(m_row / len, m_pitch / len, m_yaw / len);
  }

  EulerAngle operator+(const EulerAngle &other) const {
    return EulerAngle(m_row + other.m_row, m_pitch + other.m_pitch, m_yaw + other.m_yaw);
  }

  EulerAngle operator-(const EulerAngle &other) const {
    return EulerAngle(m_row - other.m_row, m_pitch - other.m_pitch, m_yaw - other.m_yaw);
  }

  EulerAngle operator*(T scalar) const {
    return EulerAngle(m_row * scalar, m_pitch * scalar, m_yaw * scalar);
  }

  EulerAngle operator/(T scalar) const {
    return EulerAngle(m_row / scalar, m_pitch / scalar, m_yaw / scalar);
  }

  EulerAngle &operator+=(const EulerAngle &other) {
    m_row += other.m_row;
    m_pitch += other.m_pitch;
    m_yaw += other.m_yaw;
    return *this;
  }

  EulerAngle &operator-=(const EulerAngle &other) {
    m_row -= other.m_row;
    m_pitch -= other.m_pitch;
    m_yaw -= other.m_yaw;
    return *this;
  }

  bool operator==(const EulerAngle &other) const {
    return m_row == other.m_row && m_pitch == other.m_pitch && m_yaw == other.m_yaw;
  }

  bool operator!=(const EulerAngle &other) const { return !(*this == other); }

  T Dist(const EulerAngle &other) const {
    T dx = m_row - other.m_row;
    T dy = m_pitch - other.m_pitch;
    T dz = m_yaw - other.m_yaw;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  EulerAngle &operator=(const std::array<T, 3> &arr) {
    m_row = arr[0];
    m_pitch = arr[1];
    m_yaw = arr[2];
    return *this;
  }

  template <size_t N> EulerAngle &operator=(const T (&arr)[N]) {
    static_assert(N == 3, "Array must have exactly 3 elements.");
    m_row = arr[0];
    m_pitch = arr[1];
    m_yaw = arr[2];
    return *this;
  }

private:
  T m_row, m_pitch, m_yaw;
};

} // namespace Utils

// std::formatter specialization for Utils::EulerAngle.
template <typename T> struct std::formatter<Utils::EulerAngle<T>> {
  constexpr auto parse(std::format_parse_context &ctx) const {
    return ctx.begin();
  }

  // Formats vector as "(x, y, z)".
  template <typename FormatContext>
  auto format(const Utils::EulerAngle<T> &vec, FormatContext &ctx) const {
    return std::format_to(ctx.out(), "({}, {}, {})", vec.Row(), vec.Pitch(), vec.Yaw());
  }
};
