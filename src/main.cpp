#include "config.hpp"
#include "include/game/State.hpp"
#include "include/models/Objects.hpp"
#include "include/models/Player.hpp"
#include "include/models/Scene.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <memory>

static std::shared_ptr<Game::State> gameState = Game::State::getInstance();
static Models::Player player;

// MAIN FUNCTIONS
void display();
void reshape(int w, int h);
void update(int value);

// Main entry point.
int main(int argc, char **argv) {
  // INITIAL CONFIG
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
  glutCreateWindow("OpenGL 2.1 Template");

  // CALLBACKS
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(Game::State::mouseCallback);
  glutMotionFunc(Game::State::mouseMotionCallback);
  glutPassiveMotionFunc(Game::State::mouseMotionCallback);
  glutKeyboardFunc(Game::State::keyboardCallback);
  glutKeyboardUpFunc(Game::State::keyboardUpCallback);
  glutSpecialFunc(Game::State::specialCallback);
  glutSpecialUpFunc(Game::State::specialUpCallback);

  // HIDE CURSOR
  glutSetCursor(GLUT_CURSOR_NONE);

  // LIGHTING
  glEnable(GL_LIGHTING);

  // CAMERA
  gluPerspective(60.0, 1.0 / 1.0, 1.0, 100.0);

  // LOAD TEXTURE (TODO: FIND A BETTER PLACE)
  loadTexture("./assets/wall_texture.png");

  // FPS TIMER
  glutTimerFunc(1000.0 / FPS_TARGET, update, 0);

  glutMainLoop();
  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  player.look();

  Models::Scene::drawRoom(0.0f, 0.0f, 0.0f, 20, 20, GL_LIGHT0);
  Models::Scene::drawRoom(20.0f, 0.0f, 0.0f, 20, 20, GL_LIGHT1);
  Models::Scene::drawRoom(0.0f, 0.0f, 20.0f, 20, 20, GL_LIGHT2);
  Models::Scene::drawRoom(20.0f, 0.0f, 20.0f, 20, 20, GL_LIGHT3);

  glutSwapBuffers();
}

// Update function, called periodically.
void update(int value) {
  gameState->update();
  player.update();

  glutWarpPointer(gameState->windowSize().X() / 2,
                  gameState->windowSize().Y() / 2);

  glutPostRedisplay();
  glutTimerFunc(1000.0 / FPS_TARGET, update, 0);
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
