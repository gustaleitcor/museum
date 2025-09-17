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

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

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

//multiple quads
// void Models::Objects::drawPlane(GLfloat x_0, GLfloat y_0, GLfloat z_0,
//                                 GLfloat x, GLfloat y, GLfloat z) {
//     int nx = 100; // number of quads along x
//     int ny = 100; // number of quads along y
//     glNormal3f(0, 1, 0);
//     GLfloat dx = x / nx;
//     GLfloat dy = y / ny;
//     GLfloat dz = z / ny;  // since z is coupled with y in your original code

//     GLfloat dtx = 1.0f / nx;
//     GLfloat dty = 1.0f / ny;

//     for (int i = 0; i < nx; i++) {
//         for (int j = 0; j < ny; j++) {
//             GLfloat x1 = x_0 + i * dx;
//             GLfloat y1 = y_0 + j * dy;
//             GLfloat z1 = z_0 + j * dz;

//             GLfloat x2 = x_0 + (i + 1) * dx;
//             GLfloat y2 = y_0 + (j + 1) * dy;
//             GLfloat z2 = z_0 + (j + 1) * dz;

//             GLfloat s1 = i * dtx;
//             GLfloat t1 = j * dty;
//             GLfloat s2 = (i + 1) * dtx;
//             GLfloat t2 = (j + 1) * dty;

//             glBegin(GL_QUADS);
//                 glTexCoord2f(s1, t1);
//                 glVertex3f(x1, y1, z1);

//                 glTexCoord2f(s2, t1);
//                 glVertex3f(x2, y1, z1);

//                 glTexCoord2f(s2, t2);
//                 glVertex3f(x2, y2, z2);

//                 glTexCoord2f(s1, t2);
//                 glVertex3f(x1, y2, z2);
//             glEnd();
//         }
//     }
// }
