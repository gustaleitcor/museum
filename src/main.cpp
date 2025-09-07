// Main application file for the OpenGL museum.

#define STB_IMAGE_IMPLEMENTATION

#include "include/game/State.hpp"
#include "include/models/Player.hpp"
#include "include/utils/aux.hpp"
#include "include/utils/stb_image.hpp"
#include <GL/freeglut_std.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <memory>
#include <print>

static std::shared_ptr<Game::State> gameState = Game::State::getInstance();
static Models::Player player;

static GLuint texName = 0;

// Loads an image and creates an OpenGL 2D texture.
void loadTexture(const char *filename) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (data) {
    if (texName == 0) {
      glGenTextures(1, &texName);
    }
    glBindTexture(GL_TEXTURE_2D, texName);

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
      std::println("Unsupported texture format: {} channels", nrChannels);
    }

    stbi_image_free(data);
  } else {
    std::println("Failed to load texture: {}", filename);
  }
}

// Display callback.
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  player.look();

  Utils::drawRoom();
  Utils::drawLight(0.0f, 18.0f, 0.0f);

  // std::println("{}", player);

  glutSwapBuffers();
}

// Reshape callback.
void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, static_cast<double>(w) / static_cast<double>(h), 1.0,
                 100.0);
  glMatrixMode(GL_MODELVIEW);

  gameState->mut_windowSize() = {w, h};

  glLoadIdentity();
}

// Update function, called periodically.
void update(int value) {
  glutTimerFunc(16, update, 0);

  gameState->update();
  player.update();

  glutWarpPointer(gameState->windowSize().X() / 2,
                  gameState->windowSize().Y() / 2);

  glutPostRedisplay();
}

// Main entry point.
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("OpenGL 2.1 Template");

  gluPerspective(60.0, 1.0 / 1.0, 1.0, 100.0);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(16, update, 0);

  glutSetCursor(GLUT_CURSOR_NONE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT1);

  gameState->registerCallbacks();

  loadTexture("./assets/wall_texture.png");

  glutMainLoop();
  return 0;
}
