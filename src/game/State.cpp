// Game::State class implementation.

#include "include/game/State.hpp"
#include <GL/freeglut_std.h>
#include <GL/glut.h>

void Game::State::update() {
  static bool F11_toggled = false;

  if (isSpecialPressed(Key::Special::F11)) {
    if (!F11_toggled) {
      m_isFullScreen = !m_isFullScreen;
      if (m_isFullScreen) {
        glutFullScreen();
      } else {
        glutReshapeWindow(800, 600);
      }

      F11_toggled = true;
    }
  } else {
    F11_toggled = false;
  }

  if (isKeyPressed((char)27)) {
    exit(0);
  }
}

void Game::State::reset() {
  m_mouseDelta = {0, 0};

  m_isFullScreen = false;

  for (int i = 0; i < 256; ++i) {
    m_asciiKeys[i] = false;
    m_specialKeys[i] = false;
  }
}

void Game::State::mouseCallback(int button, int state, int x, int y) {
  // TODO
}

void Game::State::mouseMotionCallback(GLint x, GLint y) {
  auto instance = getInstance();
  instance->m_mouseDelta = {x - instance->m_windowSize.X() / 2,
                            y - instance->m_windowSize.Y() / 2};
}

void Game::State::keyboardCallback(unsigned char key, int x, int y) {
  getInstance()->m_asciiKeys[key] = true;
}

void Game::State::keyboardUpCallback(unsigned char key, int x, int y) {
  getInstance()->m_asciiKeys[key] = false;

  if (key >= 'a' && key <= 'z') {
    getInstance()->m_asciiKeys[key - ('a' - 'A')] = false;
  } else if (key >= 'A' && key <= 'Z') {
    getInstance()->m_asciiKeys[key + ('a' - 'A')] = false;
  } else if (key == 0) {
    getInstance()->m_asciiKeys[32] = false;
  } else if (key == 32) {
    getInstance()->m_asciiKeys[0] = false;
  }
}

void Game::State::specialCallback(int key, int x, int y) {
  getInstance()->m_specialKeys[key] = true;
}

void Game::State::specialUpCallback(int key, int x, int y) {
  getInstance()->m_specialKeys[key] = false;
}

Game::State::State() { reset(); }
