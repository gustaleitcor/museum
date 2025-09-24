#include "include/models/Scene.hpp"
#include "include/game/State.hpp"
#include "include/models/Objects.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cstdlib>
#include <memory>
#include <iostream>

using namespace Models::Objects;
static std::shared_ptr<Game::State> gameState = Game::State::getInstance();

void Models::Scene::drawHallway(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                GLfloat w, GLfloat h, Orientation orientation, GLuint texture_id) {
  glPushMatrix();
  glNormal3f(0, 1, 0);
  glTranslatef(x_0, y_0, z_0);


  // if(texture_id %)
  int side;
  if(texture_id < 12)
  {
    side = gameState->random_number_left(1);
  }
  else
    side = gameState->random_number_right(1);
  // std::cout << "Chosen: " << side << std::endl;
  // std::cout << "W: " << w/4 << std::endl;
  // std::cout << "x_0: " << x_0 << std::endl;

  if (orientation == VERTICAL)
    glRotatef(90, 0, 1, 0);

  int frame_width = 5;
  int frame_height = 15;
  // std::cout << gameState->photo(2) << std::endl;
  if(side){
    glPushMatrix();
    glRotatef(180.0f, 0, 1, 0);
    drawFrame(0, (h - frame_height)/2, w/2 -0.4, frame_width, frame_height, gameState->photo(0));
    glPopMatrix();
  } else
    drawFrame(0, (h - frame_height)/2, w/2 - 0.4, frame_width, frame_height, gameState->photo(0));
  // Floor
  // glNormal3f(0, 1, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-w / 2, 0, -w / 2, w, 0, w, gameState->texture(4));

  // Ceiling
  // glNormal3f(0, 1, 0);
  glColor3f(0.5f, 0.5f, 0.5f);
  drawPlane(-w / 2, h, -w / 2, w, 0, w, gameState->texture(5));

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

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // Light
  // drawLight(x_0, y_0 + 0.3*h, z_0, lightName);
}

void Models::Scene::drawCorner(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat w,
                               GLfloat h, CornerType corner_type) {

  glPushMatrix();
  glTranslatef(x_0, y_0, z_0);
  glRotatef(corner_type, 0, 1, 0);
  // Floor
  glNormal3f(0, 1, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-w / 2, 0, -w / 2, w, 0, w, gameState->texture(6));

  // Ceiling
  glNormal3f(0, -1, 0);
  glColor3f(0.5f, 0.5f, 0.5f);
  drawPlane(-w / 2, h, -w / 2, w, 0, w, gameState->texture(5));

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
