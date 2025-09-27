#include "include/models/Scene.hpp"
#include "include/game/State.hpp"
#include "include/models/Objects.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <print>
// #include "../config.hpp"

using namespace Models::Objects;
static std::shared_ptr<Game::State> gameState = Game::State::getInstance();

void Models::Scene::drawHallway(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                GLfloat w, GLfloat h, Orientation orientation, GLuint texture_id)
{
  glPushMatrix();
  glNormal3f(0, 1, 0);
  glTranslatef(x_0, y_0, z_0);
  GLfloat t_x = 5.0;
  if (orientation == VERTICAL){
    glRotatef(90, 0, 1, 0);
    t_x *= -1;
  }
    

  int side;
  if (texture_id < HALL_LENGTH * 2)
  {
    side = gameState->random_number_left(1);
  }
  else
  {
    side = gameState->random_number_right(1);
  }

  GLfloat spot_direction[] = {-0.275, -0.55, -1.0};

  float scale = 1.0 * gameState->photo(texture_id).h / gameState->photo(texture_id).w;

  int frame_width = 10;
  // int frame_height = 10;
  int frame_height = ceil(frame_width * scale);
  if(!side)
  {
    spot_direction[0] *= -1;
    spot_direction[2] *= -1;
    t_x = 10;
  }


  GLfloat light1_ambient[] = {0.6, 0.6, 1.0, 1.0};
  GLfloat light1_position[] = {0.0, ROOM_HEIGHT, 0, 1.0};
  GLfloat cutoff = 25.0;
  GLfloat atte = 0.004;

  glPushMatrix();
  glTranslatef(0.0, ROOM_HEIGHT, 0);
  glutWireCube(1.0);
  glPopMatrix();

  if (int(x_0) == gameState->point_left.X() && int(z_0) == gameState->point_left.Y())
  {
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, atte);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    glEnable(GL_LIGHT1);

    std::println("Ligando left ({}, {})", x_0, z_0);
  }
  else if (int(x_0) == gameState->point_mid.X() && int(z_0) == gameState->point_mid.Y())
  {
    glLightfv(GL_LIGHT2, GL_POSITION, light1_position);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);

    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, atte);

    glLightfv(GL_LIGHT2, GL_AMBIENT, light1_ambient);

    glEnable(GL_LIGHT2);
    std::println("Ligando mid ({}, {})", x_0, z_0);
  }
  else if (int(x_0) == gameState->point_right.X() && int(z_0) == gameState->point_right.Y())
  {
    glLightfv(GL_LIGHT3, GL_POSITION, light1_position);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, cutoff);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0);

    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, atte);

    glLightfv(GL_LIGHT3, GL_AMBIENT, light1_ambient);

    glEnable(GL_LIGHT3);
    std::println("Ligando right ({}, {})", x_0, z_0);
  }
  if (side)
  {

    glPushMatrix();
    glRotatef(180.0f, 0, 1, 0);
    drawFrame(0, (h - frame_height) / 2, w / 2 - 0.4, frame_width, frame_height, gameState->photo(texture_id).id);
    drawBarrier(-2.25, 5, w / 2 - 4);
    glPopMatrix();
  }
  else
  {
    drawFrame(0, (h - frame_height) / 2, w / 2 - 0.4, frame_width, frame_height, gameState->photo(texture_id).id);
    drawBarrier(-2.25, 5, w / 2 - 4);
  }


  // Floor
  // glNormal3f(0, 1, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-w / 2, 0, -w / 2, w, 0, w, gameState->texture(6));

  // Ceiling
  // glNormal3f(0, 1, 0);
  // glColor3f(0.5f, 0.5f, 0.5f);
  // drawPlane(-w / 2, h, -w / 2, w, 0, w, gameState->texture(5));

  // Walls
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f, 1.0f, 1.0f);
  // glNormal3f(0, 1, 0);
  drawPlane(-w / 2, 0, -w / 2, w, h, 0, gameState->texture(0));
  // glNormal3f(0, 1, 0);
  drawPlane(-w / 2, 0, w / 2, w, h, 0, gameState->texture(0));

  // glRotatef(90, 0.0, 1.0, 0.0);
  // glNormal3f(0, 0, 1); //original era (1, 0, 0)
  // drawPlane(-w / 2, 0, -w / 2, w, h, 0);
  // glNormal3f(0, 0, -1);
  // drawPlane(-w / 2, 0, w / 2, w, h, 0);

  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  glDisable(GL_LIGHT3);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // Light
  // drawLight(x_0, y_0 + 0.3*h, z_0, lightName);
}

void Models::Scene::drawCorner(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat w,
                               GLfloat h, CornerType corner_type)
{

  glPushMatrix();
  glTranslatef(x_0, y_0, z_0);
  glRotatef(corner_type, 0, 1, 0);
  // Floor
  glNormal3f(0, 1, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-w / 2, 0, -w / 2, w, 0, w, gameState->texture(6));

  // Ceiling
  // glNormal3f(0, -1, 0);
  // glColor3f(0.5f, 0.5f, 0.5f);
  // drawPlane(-w / 2, h, -w / 2, w, 0, w, gameState->texture(5));

  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f, 1.0f, 1.0f);
  glNormal3f(0, 0, 1);
  drawPlane(-w / 2, 0, -w / 2, w, h, 0, gameState->texture(0));

  glRotatef(90, 0.0, 1.0, 0.0);
  glNormal3f(0, 0, 1); // original era (1, 0, 0)
  drawPlane(-w / 2, 0, -w / 2, w, h, 0, gameState->texture(0));

  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
}
