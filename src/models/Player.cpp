// Player class implementation.

#include "include/models/Player.hpp"
#include <GL/glu.h>
#include <algorithm>
#include <cmath>
#include <Vector2.hpp>

namespace Models
{

  Player::Player()
      : m_pos(0.0f, 2.5f, 0.0f), m_worldUp(0.0f, 1.0f, 0.0f),
        m_viewAngle(0.0f, 0.0f, 0.0f), m_movementSpeed(0.2f),
        m_mouseSensitivity(0.5f)
  {
    updateVectors();
  }

  void Player::look()
  {
    gluLookAt(m_pos.X(), m_pos.Y(), m_pos.Z(), m_pos.X() + m_front.X(),
              m_pos.Y() + m_front.Y(), m_pos.Z() + m_front.Z(),
              m_worldUp.X(), m_worldUp.Y(), m_worldUp.Z());
  }

  void Player::update()
  {
    auto mouseDelta = m_gameState->mouseDelta();
    m_viewAngle.mut_Z() += mouseDelta.X() * m_mouseSensitivity;
    m_viewAngle.mut_Y() -= mouseDelta.Y() * m_mouseSensitivity;

    m_viewAngle.mut_Y() = std::clamp(m_viewAngle.Y(), -89.0f, 89.0f);

    updateVectors();

    if (m_gameState->isKeyPressed('w'))
    {
      m_pos += m_front * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('W'))
    {
      m_pos += m_front * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed('s'))
    {
      m_pos -= m_front * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('S'))
    {
      m_pos -= m_front * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed('a'))
    {
      m_pos -= m_right * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('A'))
    {
      m_pos -= m_right * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed('d'))
    {
      m_pos += m_right * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('D'))
    {
      m_pos += m_right * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed(32))
    {
      m_pos.mut_Y() += m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed(0))
    {
      m_pos.mut_Y() -= m_movementSpeed;
    }

    // linha esquerda
    if (m_pos.X() <= ROOM_WIDTH / 2)
    {
      // voltando
      if (m_gameState->checkpoint() && m_pos.Z() <= (HALL_LENGTH + 1) / 2 * ROOM_WIDTH - 10)
      {
        m_gameState->toggle();
        m_gameState->sub_seed_right();
        m_gameState->load_photos(1);
      }
      // indo
      if (!m_gameState->checkpoint() && m_pos.Z() >= (HALL_LENGTH + 1) / 2 * ROOM_WIDTH + 10)
      {
        m_gameState->toggle();
        m_gameState->add_seed_right();
        m_gameState->load_photos(1);
      }
    }
    // linha direita
    if (m_pos.X() >= HALL_LENGTH * ROOM_WIDTH + ROOM_WIDTH / 2)
    {
      // indo
      if (m_gameState->checkpoint() && m_pos.Z() <= (HALL_LENGTH + 1) / 2 * ROOM_WIDTH - 10)
      {
        m_gameState->toggle();
        m_gameState->add_seed_left();
        m_gameState->load_photos(0);
      }
      // voltando
      if (!m_gameState->checkpoint() && m_pos.Z() >= (HALL_LENGTH + 1) / 2 * ROOM_WIDTH + 10)
      {
        m_gameState->toggle();
        m_gameState->sub_seed_left();
        m_gameState->load_photos(0);
      }
    }

    int x = int(m_pos.X() + ROOM_WIDTH / 2) / 30;
    int z = int(m_pos.Z() + ROOM_WIDTH / 2) / 30;

    std::cout << "x: " << x << " z: " << z << std::endl;
    Vertice v = m_gameState->get_neigh(x, z);
    std::cout << std::format("{}, {}, {}", v.room_left, v.room_mid, v.room_right) << std::endl;
    m_gameState->point_left = v.room_left;
    m_gameState->point_mid = v.room_mid;
    m_gameState->point_right = v.room_right;
    // std::println("{}", m_gameState->get_neigh(x, z));
  }

  void Player::reset()
  {
    // No specific reset logic implemented yet.
  }

  void Player::updateVectors()
  {
    Utils::Vector3<GLfloat> front;
    front.mut_X() =
        cos(m_viewAngle.Z() * M_PI / 180.0f) * cos(m_viewAngle.Y() * M_PI / 180.0f);
    front.mut_Y() = sin(m_viewAngle.Y() * M_PI / 180.0f);
    front.mut_Z() =
        sin(m_viewAngle.Z() * M_PI / 180.0f) * cos(m_viewAngle.Y() * M_PI / 180.0f);
    m_front = front.Normalized();

    m_right = m_front.Cross(m_worldUp).Normalized();
    m_up = m_right.Cross(m_front).Normalized();
  }

} // namespace Models
