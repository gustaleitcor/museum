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
#include <print>
// #include "config.hpp"

static std::shared_ptr<Game::State> gameState = Game::State::getInstance();
static Models::Player player;

// MAIN FUNCTIONS
void display();
void reshape(int w, int h);
void update(int value);

// Main entry point.
int main(int argc, char **argv)
{
  // srand(1);

  // INITIAL CONFIG
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
  glutCreateWindow("OpenGL 2.1 Template");
  // exit(1);
  



  
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  glShadeModel(GL_SMOOTH);
  // glShadeModel(GL_FLAT);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


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
  gameState->load_photos(0);
  gameState->load_photos(1);

  // HIDE CURSOR
  glutSetCursor(GLUT_CURSOR_NONE);

  // LIGHTING
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  GLfloat light_ambient[] = {0.475, 0.475, 0.475, 1.0};

  // GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 1.0};
  // GLfloat light_specular[] = {0.6, 0.6, 0.6, 1.0};
  // APENAS LUZ DIRECIONAL VINDO DE CIMA
  GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0};

  // atribui as atenuacoes
  // glLightf ( GL_LIGHT1 , GL_CONSTANT_ATTENUATION , 1.5);
  // glLightf ( GL_LIGHT1 , GL_LINEAR_ATTENUATION , 0.5);
  // glLightf ( GL_LIGHT1 , GL_QUADRATIC_ATTENUATION , 0.2);
  // atribui as caracteristicas da luz tipo lanterna
  // habilita a luz 1


  glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
  // glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
  // glLightfv(GL_LIGHT7, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT7, GL_POSITION, light_position);

  glEnable(GL_LIGHT7);
  // glEnable(GL_LIGHT1);
  // glEnable(GL_LIGHT2);
  // glEnable(GL_LIGHT3);
  glEnable(GL_MAP1_VERTEX_3);

  // CAMERA
  gluPerspective(60.0, 1.0 / 1.0, 1.0, 300.0);

  // FPS TIMER
  glutTimerFunc(1000.0 / FPS_TARGET, update, 0);

  // std::vector<std::vector<uint8_t>> images;
  // gameState->get_images(images, 1);
  gameState->reset();
  // gameState->load_photos(0);
  // std::cout << "### LOAD 0 OK ###\n";
  // gameState->load_photos(1);
  // std::cout << "### LOAD 1 OK ###\n";
  // exit(1);
  glutMainLoop();
  return 0;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  player.look();

  GLint maxLights = 0;
  glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

  // GLfloat light1_ambient[] = {0.8, 0.8, 1.0, 1.0};
  // GLfloat light1_position [] = { 60.0 + 5, ROOM_HEIGHT, -5.0 , 1.0 };
  // GLfloat spot_direction [] = { 0.0 , -1.0 , -0.5 };

  // glLightfv ( GL_LIGHT6 , GL_POSITION , light1_position);
  // glLightf (GL_LIGHT6 , GL_SPOT_CUTOFF , 20.0);
  // glLightfv (GL_LIGHT6,GL_SPOT_DIRECTION, spot_direction);
  // glLightf (GL_LIGHT6 , GL_SPOT_EXPONENT , 2.0);

  // // glLightf ( GL_LIGHT6 , GL_CONSTANT_ATTENUATION , 1.5);
  // // glLightf ( GL_LIGHT6 , GL_LINEAR_ATTENUATION , 0.5);
  // glLightf ( GL_LIGHT6 , GL_QUADRATIC_ATTENUATION , 0.004);

  // glLightfv(GL_LIGHT6, GL_AMBIENT, light1_ambient);

  // glEnable (GL_LIGHT6);

  // Models::Objects::drawLight(0.0, 10, 0.0, GL_LIGHT0);
  // printf("Maximum number of lights supported: %d\n", maxLights);

  // Models::Objects::drawFrame(0.0, 3.0f, 0.0, 3, 4,  gameState->texture(3));
  // Models::Objects::drawPlaneFlat(0.0, 5.0f, 0.0, 5, 5, 0, gameState->texture(0));
  // sky
  glDisable(GL_DEPTH_TEST);
  glPushMatrix();
  // Models::Objects::drawPlaneFlat(-30 * ROOM_WIDTH / 2, ROOM_HEIGHT * 2, -30 * ROOM_WIDTH / 2, 15 * HALL_LENGTH * ROOM_WIDTH, 0, 15 * HALL_LENGTH * ROOM_WIDTH, gameState->texture(9));
  const float sky_side = 40;
  Models::Objects::drawPlaneFlat(player.Pos().X()-sky_side/2, player.Pos().Y()+1.2, player.Pos().Z()-sky_side/2, sky_side, 0, sky_side, gameState->texture(9));
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);

  int count = 0;
  // Corredor horizontal superior
  for (int i = 0; i < HALL_LENGTH; i++)
  {
    Models::Scene::drawHallway(ROOM_WIDTH + ROOM_WIDTH * i, 0.0, 0.0, ROOM_WIDTH, ROOM_HEIGHT, HORIZONTAL, count);
    count++;
  }
  // Canto superior esquerdo  |-
  Models::Scene::drawCorner(0.0, 0.0, 0.0, ROOM_WIDTH, ROOM_HEIGHT, UPPER_LEFT);
  // Corredor vertical esquerdo
  for (int i = 0; i < HALL_LENGTH; i++)
  {
    Models::Scene::drawHallway(0.0, 0.0, ROOM_WIDTH + ROOM_WIDTH * i, ROOM_WIDTH, ROOM_HEIGHT, VERTICAL, count);
    count++;
  }
  // Canto inferior esquerdo
  Models::Scene::drawCorner(0.0, 0.0, ROOM_WIDTH + ROOM_WIDTH * HALL_LENGTH, ROOM_WIDTH, ROOM_HEIGHT, BOTTOM_LEFT);
  // Canto superior direito
  Models::Scene::drawCorner(ROOM_WIDTH + ROOM_WIDTH * HALL_LENGTH, 0.0, 0.0, ROOM_WIDTH, ROOM_HEIGHT, UPPER_RIGHT);
  // Corredor vertical direito
  // int j = 0;
  for (int j = 0; j < HALL_LENGTH; j++)
  {
    Models::Scene::drawHallway(ROOM_WIDTH + ROOM_WIDTH * HALL_LENGTH, 0.0, ROOM_WIDTH + ROOM_WIDTH * j, ROOM_WIDTH, ROOM_HEIGHT, VERTICAL, count);
    count++;
  }
  // Canto inferior direito
  Models::Scene::drawCorner(ROOM_WIDTH + ROOM_WIDTH * HALL_LENGTH, 0.0, ROOM_WIDTH + ROOM_WIDTH * HALL_LENGTH, ROOM_WIDTH, ROOM_HEIGHT,
                            BOTTOM_RIGHT);
  // Corredor horizontal inferior
  for (int i = 0; i < HALL_LENGTH; i++)
  {
    Models::Scene::drawHallway(ROOM_WIDTH + ROOM_WIDTH * i, 0.0, ROOM_WIDTH + ROOM_WIDTH * HALL_LENGTH, ROOM_WIDTH, ROOM_HEIGHT, HORIZONTAL, count);
    count++;
  }
  // Models::Scene::drawRoom(20.0, 0.0, 0.0, 20, 20, GL_LIGHT1);
  // Models::Scene::drawRoom(0.0, 0.0, 20.0, 20, 20, GL_LIGHT2);
  // Models::Scene::drawRoom(20.0, 0.0, 20.0, 20, 20, GL_LIGHT3);
  glPushMatrix();
  if (gameState->checkpoint())
    glTranslatef(0, 5, (HALL_LENGTH + 1) / 2 * ROOM_WIDTH - 10);
  else
    glTranslatef(0, 5, (HALL_LENGTH + 1) / 2 * ROOM_WIDTH + 10);
  glColor3f(0, 1, 0);
  glScalef(ROOM_WIDTH, 1, 1);
  glutWireCube(1.0);
  glPopMatrix();

  glPushMatrix();
  if (gameState->checkpoint())
    glTranslatef(HALL_LENGTH * ROOM_WIDTH + ROOM_WIDTH, 5, (HALL_LENGTH + 1) / 2 * ROOM_WIDTH - 10);
  else
    glTranslatef(HALL_LENGTH * ROOM_WIDTH + ROOM_WIDTH, 5, (HALL_LENGTH + 1) / 2 * ROOM_WIDTH + 10);
  glColor3f(0, 1, 0);
  glScalef(ROOM_WIDTH, 1, 1);
  glutWireCube(1.0);
  glPopMatrix();

  gameState->reset();

  glutSwapBuffers();
}

// Update function, called periodically.
void update(int value)
{
  gameState->update();
  player.update();
  std::println("player: {}", player);

  glutWarpPointer(gameState->windowSize().X() / 2,
                  gameState->windowSize().Y() / 2);

  glutPostRedisplay();
  glutTimerFunc(1000.0 / FPS_TARGET, update, 0);
}

// Reshape callback.
void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, static_cast<double>(w) / static_cast<double>(h), 1.0,
                 20 * ROOM_WIDTH);
  glMatrixMode(GL_MODELVIEW);

  gameState->mut_windowSize() = {w, h};

  glLoadIdentity();
}
