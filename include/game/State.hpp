#pragma once

#include "Vector2.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <memory>

#define MAX_TEXTURES 4

// Namespace for key-related definitions.
namespace Key {
// Enum to map GLUT special key codes.
enum Special {
  F1 = GLUT_KEY_F1,
  F2 = GLUT_KEY_F2,
  F3 = GLUT_KEY_F3,
  F4 = GLUT_KEY_F4,
  F5 = GLUT_KEY_F5,
  F6 = GLUT_KEY_F6,
  F7 = GLUT_KEY_F7,
  F8 = GLUT_KEY_F8,
  F9 = GLUT_KEY_F9,
  F10 = GLUT_KEY_F10,
  F11 = GLUT_KEY_F11,
  F12 = GLUT_KEY_F12,
  LEFT = GLUT_KEY_LEFT,
  UP = GLUT_KEY_UP,
  RIGHT = GLUT_KEY_RIGHT,
  DOWN = GLUT_KEY_DOWN,
  PAGE_UP = GLUT_KEY_PAGE_UP,
  PAGE_DOWN = GLUT_KEY_PAGE_DOWN,
  HOME = GLUT_KEY_HOME,
  END = GLUT_KEY_END,
  INSERT = GLUT_KEY_INSERT
};
} // namespace Key

// Namespace for game-related components.
namespace Game {
// Manages global game state (input, window properties) as a Singleton.
class State {
public:
  // Returns the singleton instance.
  static std::shared_ptr<State> getInstance() {
    static std::shared_ptr<State> instance{new State()};
    return instance;
  }

  inline const Utils::Vector2<int> &mouseDelta() const { return m_mouseDelta; }
  inline Utils::Vector2<int> &mut_mouseDelta() { return m_mouseDelta; }

  inline const Utils::Vector2<int> &windowSize() const { return m_windowSize; }
  inline Utils::Vector2<int> &mut_windowSize() { return m_windowSize; }

  inline const GLuint texture(int i) const { return m_texIds[i]; }

  // Updates game state.
  void update();
  // Resets game state.
  void reset();
  // Load texture
  void load_texture();

  // Mouse button callback.
  static void mouseCallback(int button, int state, int x, int y);
  // Mouse motion callback.
  static void mouseMotionCallback(GLint x, GLint y);
  // ASCII key press callback.
  static void keyboardCallback(unsigned char key, int x, int y);
  // ASCII key release callback.
  static void keyboardUpCallback(unsigned char key, int x, int y);
  // Special key press callback.
  static void specialCallback(int key, int x, int y);
  // Special key release callback.
  static void specialUpCallback(int key, int x, int y);

  // Checks if ASCII key is pressed.
  inline bool isKeyPressed(char key) const {
    return m_asciiKeys[static_cast<unsigned char>(key)];
  }

  // Checks if special key is pressed.
  inline bool isSpecialPressed(Key::Special key) const {
    return m_specialKeys[key];
  }

private:
  State();
  State(const State &) = delete;
  State &operator=(const State &) = delete;

  Utils::Vector2<int> m_mouseDelta;

  bool m_asciiKeys[256];
  bool m_specialKeys[256];

  Utils::Vector2<int> m_windowSize;
  bool m_isFullScreen;

  GLuint m_texIds[MAX_TEXTURES] = {0};
};
} // namespace Game
