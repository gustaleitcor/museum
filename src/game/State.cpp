// Game::State class implementation.

#include "include/game/State.hpp"
#include <GL/freeglut_std.h>
#include <GL/glut.h>

void Game::State::update() {
  if (isSpecialPressed(Key::Special::F11)) {
    int currentWidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    if (currentWidth == screenWidth && currentHeight == screenHeight) {
      glutReshapeWindow(800, 600);
    } else {
      glutFullScreen();
    }
  }

  if (isKeyPressed((char)27)) {
    exit(0);
  }
}

void Game::State::reset() {
  m_mouseDelta = {0, 0};

  for (int i = 0; i < 256; ++i) {
    m_asciiKeys[i] = false;
    m_specialKeys[i] = false;
  }
}

void Game::State::registerCallbacks() {
  glutMouseFunc([](int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      getInstance()->m_lastMousePos = {x, y};
    }
  });
  glutMotionFunc(mouseCallback);
  glutPassiveMotionFunc(mouseCallback);
  glutKeyboardFunc(keyboardCallback);
  glutKeyboardUpFunc(keyboardUpCallback);
  glutSpecialFunc(specialCallback);
  glutSpecialUpFunc(specialUpCallback);
}

void Game::State::mouseCallback(GLint x, GLint y) {
  auto instance = getInstance();
  instance->m_mouseDelta = {x - instance->m_windowSize.X() / 2,
                            y - instance->m_windowSize.Y() / 2};
  instance->m_lastMousePos = {x, y};
}

void Game::State::keyboardCallback(unsigned char key, int x, int y) {
  getInstance()->m_asciiKeys[key] = true;
}

void Game::State::keyboardUpCallback(unsigned char key, int x, int y) {
  getInstance()->m_asciiKeys[key] = false;
}

void Game::State::specialCallback(int key, int x, int y) {
  getInstance()->m_specialKeys[key] = true;
}

void Game::State::specialUpCallback(int key, int x, int y) {
  getInstance()->m_specialKeys[key] = false;
}

Game::State::State() { reset(); }