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
                                GLfloat w, GLfloat h, Orientation orientation) {
  glPushMatrix();
  glNormal3f(0, 1, 0);
  glTranslatef(x_0, y_0, z_0);
  if (orientation == VERTICAL)
    glRotatef(90, 0, 1, 0);
  // Floor
  // glNormal3f(0, 1, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-w / 2, 0, -w / 2, w, 0, w, gameState->texture(1));

  // Ceiling
  // glNormal3f(0, 1, 0);
  glColor3f(0.5f, 0.5f, 0.5f);
  drawPlane(-w / 2, h, -w / 2, w, 0, w, gameState->texture(1));

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
  drawPlane(-w / 2, 0, -w / 2, w, 0, w, gameState->texture(0));

  // Ceiling
  glNormal3f(0, -1, 0);
  glColor3f(0.5f, 0.5f, 0.5f);
  drawPlane(-w / 2, h, -w / 2, w, 0, w, gameState->texture(0));

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
