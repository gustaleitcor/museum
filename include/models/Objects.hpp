#include <GL/gl.h>

namespace Models {
namespace Objects {

void drawLight(GLfloat x_0, GLfloat y_0, GLfloat z_0, int lightName);
void drawPlane(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat x, GLfloat y,
               GLfloat z, GLuint texture);
void drawPlaneFlat(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat x, GLfloat y, GLfloat z, GLuint texture);
void drawFrame(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat w,
               GLfloat h,  GLuint texture_photo);
} // namespace Objects
} // namespace Models
