#include "include/utils/aux.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>

void Utils::drawRoom() {
  // Floor
  glColor3f(1.0f, 1.0f, 1.0f);
  drawPlane(-10.0f, 0.0f, -10.0f, 20.0f, 0.0f, 20.0f);
  // Ceiling
  glColor3f(0.5f, 0.5f, 0.5f);
  drawPlane(-10.0f, 20.0f, -10.0f, 20.0f, 0.0f, 20.0f);

  // Walls
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f, 1.0f, 1.0f);
  // glColor3f(0.0f, 0.5f, 0.5f);
  drawPlane(-10.0f, 0.0f, -10.0f, 20.0f, 20.0f, 0.0f);
  // glColor3f(0.0f, 0.0f, 0.5f);
  drawPlane(-10.0f, 0.0f, 10.0f, 20.0f, 20.0f, 0.0f);

  glRotatef(90, 0.0, 1.0, 0.0);

  // glColor3f(0.5f, 0.0f, 0.5f);
  drawPlane(-10.0f, 0.0f, -10.0f, 20.0f, 20.0f, 0.0f);
  // glColor3f(0.5f, 0.5f, 0.0f);
  drawPlane(-10.0f, 0.0f, 10.0f, 20.0f, 20.0f, 0.0f);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void Utils::drawPlane(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat x,
                      GLfloat y, GLfloat z) {
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(x_0, y_0, z_0);

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(x_0 + x, y_0, z_0);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(x_0 + x, y_0 + y, z_0 + z);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(x_0, y_0 + y, z_0 + z);
  glEnd();
}

void Utils::drawLight(GLfloat x_0, GLfloat y_0, GLfloat z_0) {
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  glShadeModel(GL_SMOOTH);
  // glShadeModel(GL_FLAT);

  glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

  GLfloat light1_ambient[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light1_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light1_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light1_position[] = {x_0, y_0, z_0, 1.0};

  glPushMatrix();
  glDisable(GL_LIGHTING);
  glTranslatef(x_0, y_0, z_0);
  glColor3f(1.0f, 1.0f, 0.0f);
  glutWireCube(1.0f);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
}
