#include "include/models/Scene.hpp"
#include "include/models/Objects.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>

using namespace Models::Objects;

void Models::Scene::drawRoom(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat w,
                             GLfloat h, int lightName) {
  glPushMatrix();
  glTranslatef(x_0, y_0, z_0);

  // Floor
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-w / 2, 0, -w / 2, w, 0, w);
  // Ceiling
  glColor3f(0.5f, 0.5f, 0.5f);
  drawPlane(-w / 2, h, -w / 2, w, 0, w);

  // Walls
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f, 1.0f, 1.0f);

  drawPlane(-w / 2, 0, -w / 2, w, h, 0);
  drawPlane(-w / 2, 0, w / 2, w, h, 0);

  glRotatef(90, 0.0, 1.0, 0.0);

  drawPlane(-w / 2, 0, -w / 2, w, h, 0);
  drawPlane(-w / 2, 0, w / 2, w, h, 0);

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // Light
  drawLight(x_0, y_0 + h * 0.8, z_0, lightName);
}
