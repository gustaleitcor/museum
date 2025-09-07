#pragma once

#include "include/game/State.hpp"
#include "include/utils/Vector3.hpp"
#include <GL/gl.h>
#include <format>
#include <memory>

namespace Models {
// Player entity.
struct Player {
public:
  Player();

  inline const Utils::Vector3<GLfloat> &Pos() const { return m_pos; }

  // Applies camera transformations.
  void look();
  // Updates player state.
  void update();
  // Resets player position and orientation.
  void reset();

private:
  // Updates front, up, and right vectors.
  void updateVectors();

  Utils::Vector3<GLfloat> m_pos;
  Utils::Vector3<GLfloat> m_front;
  Utils::Vector3<GLfloat> m_up;
  Utils::Vector3<GLfloat> m_right;
  Utils::Vector3<GLfloat> m_worldUp;

  GLfloat m_yaw;
  GLfloat m_pitch;

  GLfloat m_movementSpeed;
  GLfloat m_mouseSensitivity;

  // Game state for input handling.
  std::shared_ptr<Game::State> m_gameState = Game::State::getInstance();
};
} // namespace Models

// std::formatter specialization for Models::Player.
template <> struct std::formatter<Models::Player> {
  constexpr auto parse(std::format_parse_context &ctx) const {
    return ctx.begin();
  }

  // Formats player position.
  template <typename FormatContext>
  auto format(const Models::Player &player, FormatContext &ctx) const {
    return std::format_to(ctx.out(), "Player{{pos={}}}", player.Pos());
  }
};
