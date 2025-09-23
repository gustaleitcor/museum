#include "include/game/State.hpp"
#include "include/models/Objects.hpp"
#include "include/models/Player.hpp"
#include "include/models/Scene.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <memory>
#include <iostream>

#define HALL_LENGTH 6
#define INITIAL_WINDOW_WIDTH 800
#define INITIAL_WINDOW_HEIGHT 600
#define FPS_TARGET 60.0f

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

  gameState->load_texture();

  // HIDE CURSOR
  glutSetCursor(GLUT_CURSOR_NONE);

  // LIGHTING
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
  // GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 1.0};
  // GLfloat light_specular[] = {0.6, 0.6, 0.6, 1.0};
  GLfloat light_position[] = {0.0, 1.0, 0.0, 0.0};

  glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
  // glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
  // glLightfv(GL_LIGHT7, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT7, GL_POSITION, light_position);

  glEnable(GL_LIGHT7);

  // CAMERA
  gluPerspective(60.0, 1.0 / 1.0, 1.0, 300.0);

  // FPS TIMER
  glutTimerFunc(1000.0 / FPS_TARGET, update, 0);

  glutMainLoop();
  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  player.look();

  GLint maxLights = 0;
  glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
  // Models::Objects::drawLight(0.0, 10, 0.0, GL_LIGHT0);
  // printf("Maximum number of lights supported: %d\n", maxLights);

  Models::Objects::drawFrame(0.0f, 3.0f, 0.0f, 3, 4,  gameState->texture(3));
  // Models::Objects::drawPlaneFlat(0.0f, 5.0f, 0.0f, 5, 5, 0, gameState->texture(0));
  // Corredor horizontal superior
  int i = 0;
  for (int i = 0; i < HALL_LENGTH; i++) {
    Models::Scene::drawHallway(30.0 + 30 * i, 0.0, 0.0, 30, 25, HORIZONTAL);
  }
  // Canto superior esquerdo  |-
  Models::Scene::drawCorner(0.0f, 0.0f, 0.0f, 30, 25, UPPER_LEFT);
  // Corredor vertical esquerdo
  for (i = 0; i < HALL_LENGTH; i++) {
    Models::Scene::drawHallway(0.0, 0.0, 30.0 + 30 * i, 30, 25, VERTICAL);
  }
  // Canto inferior esquerdo
  Models::Scene::drawCorner(0.0f, 0.0f, 30.0 + 30 * i, 30, 25, BOTTOM_LEFT);
  // Canto superior direito
  Models::Scene::drawCorner(30.0 + 30 * i, 0.0f, 0.0f, 30, 25, UPPER_RIGHT);
  // Corredor vertical direito
  int j = 0;
  for (j = 0; j < HALL_LENGTH; j++) {
    Models::Scene::drawHallway(30.0 + 30 * i, 0.0, 30.0 + 30 * j, 30, 25,
                               VERTICAL);
  }
  // Canto inferior direito
  Models::Scene::drawCorner(30.0 + 30 * i, 0.0f, 30.0 + 30 * j, 30, 25,
                            BOTTOM_RIGHT);
  // Corredor horizontal inferior
  for (i = 0; i < HALL_LENGTH; i++) {
    Models::Scene::drawHallway(30.0 + 30 * i, 0.0, 30.0 + 30 * j, 30, 25,
                               HORIZONTAL);
  }
  // Models::Scene::drawRoom(20.0f, 0.0f, 0.0f, 20, 20, GL_LIGHT1);
  // Models::Scene::drawRoom(0.0f, 0.0f, 20.0f, 20, 20, GL_LIGHT2);
  // Models::Scene::drawRoom(20.0f, 0.0f, 20.0f, 20, 20, GL_LIGHT3);

  glutSwapBuffers();
}

// Update function, called periodically.
void update(int value) {
  gameState->update();
  player.update();
  // std::println("player: {}", player);

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
                 300.0);
  glMatrixMode(GL_MODELVIEW);

  gameState->mut_windowSize() = {w, h};

  glLoadIdentity();
}
