#include "include/models/Objects.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>

void Models::Objects::drawLight(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                int lightName) {
  glEnable(lightName);

  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  glShadeModel(GL_SMOOTH);
  // glShadeModel(GL_FLAT);

  glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

  GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {x_0, y_0, z_0, 1.0};

  glPushMatrix();
  glDisable(GL_LIGHTING);
  glTranslatef(x_0, y_0, z_0);
  glColor3f(1.0f, 1.0f, 0.0f);
  glutWireCube(1.0f);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  glLightfv(lightName, GL_AMBIENT, light_ambient);
  glLightfv(lightName, GL_DIFFUSE, light_diffuse);
  glLightfv(lightName, GL_SPECULAR, light_specular);
  glLightfv(lightName, GL_POSITION, light_position);
}

void Models::Objects::drawPlane(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                GLfloat x, GLfloat y, GLfloat z) {
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
