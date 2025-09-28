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

static std::shared_ptr<Game::State> gameState = Game::State::getInstance();
static Models::Player player;

// MAIN FUNCTIONS
void display();
void reshape(int w, int h);
void update(int value);

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
  glutCreateWindow("OpenGL 2.1 Template");
  glutSetCursor(GLUT_CURSOR_NONE);

  // Ativa para o uso de curvas de Bezier
  glEnable(GL_MAP1_VERTEX_3);

  // Ativa o Z-buffer
  glEnable(GL_DEPTH_TEST);

  // Camera
  gluPerspective(60.0, 1.0 / 1.0, 1.0, 300.0);
  
  // Parametros de luz
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glShadeModel(GL_SMOOTH);

  // FPS controle
  glutTimerFunc(1000.0 / FPS_TARGET, update, 0);

  // Material
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  // Callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMotionFunc(Game::State::mouseMotionCallback);
  glutPassiveMotionFunc(Game::State::mouseMotionCallback);
  glutKeyboardFunc(Game::State::keyboardCallback);
  glutKeyboardUpFunc(Game::State::keyboardUpCallback);
  glutSpecialFunc(Game::State::specialCallback);
  glutSpecialUpFunc(Game::State::specialUpCallback);

  // Carregando texturas iniciais (parede, chao e quadros iniciais)
  gameState->load_texture();
  gameState->load_photos(0);
  gameState->load_photos(1);

  // APENAS LUZ DIRECIONAL VINDO DE CIMA
  GLfloat light_ambient[] = {0.475, 0.475, 0.475, 1.0};
  GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0};
  glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT7, GL_POSITION, light_position);
  glEnable(GL_LIGHT7);

  glutMainLoop();
  return 0;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  player.look();

  // Desenha o ceu acima do jogador
  glDisable(GL_DEPTH_TEST);
  glPushMatrix();
  const float sky_length = 40;
  Models::Objects::drawPlaneFlat(player.Pos().X()-sky_length/2, player.Pos().Y()+1.2, player.Pos().Z()-sky_length/2, sky_length, 0, sky_length, gameState->texture(9));
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);

  // Renderização do museu
  int count = 0;
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

// A cada 16ms (60fps) a função update é invocada
void update(int value)
{
  gameState->update();
  player.update();
  
  // Retorna o cursor para o centro da tela
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
                 300.0);
  glMatrixMode(GL_MODELVIEW);

  gameState->mut_windowSize() = {w, h};

  glLoadIdentity();
}
