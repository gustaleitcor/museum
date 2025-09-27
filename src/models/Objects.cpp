#include "include/models/Objects.hpp"
#include "include/game/State.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>

static std::shared_ptr<Game::State> gameState = Game::State::getInstance();

void Models::Objects::drawLight(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                int lightName)
{
  glEnable(lightName);

  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  // glShadeModel(GL_SMOOTH);
  glShadeModel(GL_FLAT);

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

void Models::Objects::drawPlaneFlat(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat x, GLfloat y, GLfloat z, GLuint texture)
{

  glColor3f(1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(x_0, y_0, z_0);

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(x_0 + x, y_0, z_0);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(x_0 + x, y_0 + y, z_0 + z);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(x_0, y_0 + y, z_0 + z);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

// multiple quads
void Models::Objects::drawPlane(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                GLfloat x, GLfloat y, GLfloat z,
                                GLuint texture)
{
  int nx = 20; // number of quads along x
  int ny = 20; // number of quads along y
  // glNormal3f(0, 1, 0);
  GLfloat dx = x / nx;
  GLfloat dy = y / ny;
  GLfloat dz = z / ny; // since z is coupled with y in your original code

  GLfloat dtx = 1.0f / nx;
  GLfloat dty = 1.0f / ny;

  for (int i = 0; i < nx; i++)
  {
    for (int j = 0; j < ny; j++)
    {
      GLfloat x1 = x_0 + i * dx;
      GLfloat y1 = y_0 + j * dy;
      GLfloat z1 = z_0 + j * dz;

      GLfloat x2 = x_0 + (i + 1) * dx;
      GLfloat y2 = y_0 + (j + 1) * dy;
      GLfloat z2 = z_0 + (j + 1) * dz;

      GLfloat s1 = i * dtx;
      GLfloat t1 = j * dty;
      GLfloat s2 = (i + 1) * dtx;
      GLfloat t2 = (j + 1) * dty;

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture);
      glBegin(GL_QUADS);
      glTexCoord2f(s1, t1);
      glVertex3f(x1, y1, z1);

      glTexCoord2f(s2, t1);
      glVertex3f(x2, y1, z1);

      glTexCoord2f(s2, t2);
      glVertex3f(x2, y2, z2);

      glTexCoord2f(s1, t2);
      glVertex3f(x1, y2, z2);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
  }
}

void Models::Objects::drawFrame(GLfloat x_0, GLfloat y_0, GLfloat z_0,
                                GLfloat w, GLfloat h, GLuint texture_photo)
{

  glPushMatrix();
  glTranslatef(x_0, y_0, z_0);
  float border_width = w * 0.1;
  float border_incline = 0.25;
  // Foto
  drawPlaneFlat(0, 0, 0, w, h, 0, texture_photo);

  // Borda superior
  drawPlaneFlat(0, h, 0, w, border_width, border_incline, gameState->texture(1));

  // Borda esquerda
  glPushMatrix();
  glRotatef(45, 0, 1, 0);
  drawPlaneFlat(-border_width, 0, 0, border_width, h, 0, gameState->texture(1));
  glPopMatrix();

  // Canto superior esquerdo
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gameState->texture(1));
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.707 * border_width, h, 0.707 * border_width);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0, h, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0, h + border_width, border_incline);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  // Borda inferior
  drawPlaneFlat(0, -border_width, border_incline, w, border_width, -border_incline, gameState->texture(1));
  // Canto superior direito
  // Borda direita
  glPushMatrix();
  glTranslatef(w, 0, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gameState->texture(1));
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.707 * border_width, h, 0.707 * border_width);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0, h, 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0, h + border_width, border_incline);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glRotatef(-45, 0, 1, 0);
  drawPlaneFlat(0, 0, 0, border_width, h, 0, gameState->texture(1));
  glPopMatrix();

  // Canto inferior esquerdo
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gameState->texture(1));
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0, -border_width, border_incline);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.707 * border_width, 0, 0.707 * border_width);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0, 0, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  // Canto inferior direito
  glPushMatrix();
  glTranslatef(w, 0, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gameState->texture(1));
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0, -border_width, border_incline);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.707 * border_width, 0, 0.707 * border_width);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0, 0, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  glPopMatrix();
}

void Models::Objects::drawBarrier(GLfloat x_0, GLfloat y_0, GLfloat z_0)
{
  GLfloat ctrlpoints[4][3] = {{1, 0.5, 0.0}, {1, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0, 0.5, 0.0}};
  int points = 50;
  glPushMatrix();
  glTranslatef(x_0, y_0, z_0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, gameState->texture(8));

  // corda
  glPushMatrix();
  glScalef(ROOM_WIDTH/2, ROOM_HEIGHT/3, 1);
  glLineWidth(10);
  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= points; i++)
  {
    glEvalCoord1f((GLfloat)1.0 * i / points);
  }
  glEnd();
  glPopMatrix();

  GLUquadricObj *objCylinder = gluNewQuadric();
  gluQuadricTexture ( objCylinder , GL_TRUE );
  glColor3f(1.0, 215.0/255, 0.0);
  // poste esquerdo
  glPushMatrix();
  
  glTranslatef(0, ROOM_HEIGHT/6 - 0.5, 0);
  glPushMatrix();
  glRotatef(90.0, 1, 0, 0);
  gluCylinder(objCylinder, 0.5, 0.5, ROOM_HEIGHT/3, 10, 5);  
  glTranslatef(0, 0, ROOM_HEIGHT/3);
  gluDisk(objCylinder, 0, 1.5, 15, 10); 
  gluCylinder(objCylinder, 1.5, 1.5, 0.3, 15, 10);  
  glPopMatrix();
  gluSphere(objCylinder, 0.8, 30, 20);
  glPopMatrix();

  // poster direito
  glPushMatrix();
  glTranslatef(ROOM_WIDTH/2, ROOM_HEIGHT/6 - 0.5, 0);
  glPushMatrix();
  glRotatef(90.0, 1, 0, 0);
  gluCylinder(objCylinder, 0.5, 0.5, ROOM_HEIGHT/3, 10, 5);  
  glTranslatef(0, 0, ROOM_HEIGHT/3);
  gluDisk(objCylinder, 0, 1.5, 15, 10); 
  gluCylinder(objCylinder, 1.5, 1.5, 0.3, 15, 10);  
  glPopMatrix();
  gluSphere(objCylinder, 0.8, 30, 20);
  glPopMatrix();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
  gluDeleteQuadric(objCylinder);
}
