// Game::State class implementation.

#include "include/game/State.hpp"
#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <format>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "include/utils/stb_image.hpp"

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

Game::State::State() {
  reset();
}

void Game::State::load_texture() {
  glGenTextures(MAX_TEXTURES, m_texIds);

  for (int i = 0; i < MAX_TEXTURES; i++) {
    int width, height, nrChannels;
    std::string image_path = std::format("assets/textures/{}.png", i + 1);
    unsigned char *data =
        stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
      glBindTexture(GL_TEXTURE_2D, m_texIds[i]);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (nrChannels == 3) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
      } else if (nrChannels == 4) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);
      } else {
        std::cout << std::format("Unsupported texture format: {} channels",
                                 nrChannels)
                  << std::endl;
      }

      stbi_image_free(data);
    } else {
      std::cout << std::format("Failed to load texture: {}", image_path)
                << std::endl;
    }
  }
}
