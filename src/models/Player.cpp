// Player class implementation.

#include "include/models/Player.hpp"
#include <GL/glu.h>
#include <algorithm>
#include <cmath>
#include <Vector2.hpp>

namespace Models
{

  Player::Player()
      : m_pos(0.0f, ROOM_HEIGHT/2, 0.0f), m_worldUp(0.0f, 1.0f, 0.0f),
        m_viewAngle(0.0f, 0.0f, 0.0f), m_movementSpeed(0.3f),
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
    // Camera do jogador (mouse)
    // Calcula orientação da camera baseado no movimento do mouse
    Utils::Vector2<int> mouseDelta = m_gameState->mouseDelta();
    m_viewAngle.mut_Yaw() += mouseDelta.X() * m_mouseSensitivity;
    m_viewAngle.mut_Pitch() -= mouseDelta.Y() * m_mouseSensitivity;

    m_viewAngle.mut_Pitch() = std::clamp(m_viewAngle.Pitch(), -89.0f, 89.0f);
    updateVectors();

    // Movimentação do jogador (WASD)
    // Calcula a proxima posição do jogador
    Utils::Vector3<GLfloat> future_pos = m_pos;
    bool collision_x = false, collision_z = false;
    if (m_gameState->isKeyPressed('w'))
    {
      future_pos += m_front * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('W'))
    {
      future_pos += m_front * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed('s'))
    {
      future_pos -= m_front * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('S'))
    {
      future_pos -= m_front * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed('a'))
    {
      future_pos -= m_right * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('A'))
    {
      future_pos -= m_right * m_movementSpeed * 4;
    }

    if (m_gameState->isKeyPressed('d'))
    {
      future_pos += m_right * m_movementSpeed;
    }
    else if (m_gameState->isKeyPressed('D'))
    {
      future_pos += m_right * m_movementSpeed * 4;
    }

    // Coodernadas das salas (usado para acender as luzes proximas)
    int x = int(future_pos.X() + ROOM_WIDTH / 2) / 30;
    int z = int(future_pos.Z() + ROOM_WIDTH / 2) / 30;

    // Checagem de colisão
    if(future_pos.Z() > ROOM_WIDTH/2 - 1 && future_pos.Z() < HALL_LENGTH*ROOM_WIDTH  + ROOM_WIDTH/2+ 2)
    {
      if(future_pos.X() >= 1.0f * ROOM_WIDTH / 2 - 2 && x == 0)
      {
        collision_x = true;
      }
      if(future_pos.X() <= 1.0f * HALL_LENGTH*ROOM_WIDTH + ROOM_WIDTH/2 + 2 && x == HALL_LENGTH + 1)
      {
        collision_x = true;
      }
    }
    if(future_pos.X() > ROOM_WIDTH/2 - 1 && future_pos.X() < HALL_LENGTH*ROOM_WIDTH  + ROOM_WIDTH/2+ 2)
    {
       if(future_pos.Z() >= 1.0f * ROOM_WIDTH / 2 - 2 && z == 0)
      {
        collision_z = true;
      }
      if(future_pos.Z() <= 1.0f * HALL_LENGTH*ROOM_WIDTH + ROOM_WIDTH/2 + 2 && z == HALL_LENGTH + 1)
      {
        collision_z = true;
      }
    }
    
    future_pos.mut_Z() = std::clamp(future_pos.Z(), -1.0f*ROOM_WIDTH/2 + 2, -2 + 1.0f* (HALL_LENGTH + 1) * ROOM_WIDTH + ROOM_WIDTH/2);
    future_pos.mut_X() = std::clamp(future_pos.X(), -1.0f * ROOM_WIDTH/2 + 2, -2 + 1.0f* (HALL_LENGTH + 1) * ROOM_WIDTH + ROOM_WIDTH/2);

    // Se não colidir em uma parede em um dos eixos, seu valor será atualizado
    if(!collision_x)
    {
      m_pos.mut_X() = future_pos.X();
    }
    if(!collision_z)
    {
      m_pos.mut_Z() = future_pos.Z();
    }

    // Atualização das seed do game state baseado se o jogador se o jogador está "indo" ou "voltando"
    if (m_pos.X() <= ROOM_WIDTH / 2) // linha esquerda
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
    if (m_pos.X() >= HALL_LENGTH * ROOM_WIDTH + ROOM_WIDTH / 2) // linha direita
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

    // Calculo para pegar as 3 luzes mais proximas do jogador  
    Vertice v = m_gameState->get_neigh(x, z);
    m_gameState->mut_point_left() = v.room_left;
    m_gameState->mut_point_mid() = v.room_mid;
    m_gameState->mut_point_right() = v.room_right;
  }

  void Player::updateVectors()
  {
    Utils::Vector3<GLfloat> front;
    
    // Converte os angulos da camera de angulo para radianos
    GLfloat YawRad = m_viewAngle.Yaw() * M_PI / 180.0f;
    GLfloat PitchRad = m_viewAngle.Pitch() * M_PI / 180.0f;
     
    // Converte on angulos de Euler na direção frontal cartesiana
    // https://learnopengl.com/Getting-started/Camera
    front.mut_X() = cos(YawRad) * cos(PitchRad);
    front.mut_Y() = sin(PitchRad);
    front.mut_Z() = sin(YawRad) * cos(PitchRad);
    m_front = front.Normalized();
    
    // Calculo dos vetores de direção da camera (direita e esquerda)
    m_right = m_front.Cross(m_worldUp).Normalized();
  }
} // namespace Models
