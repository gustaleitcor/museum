#include <GL/gl.h>

namespace Models {
namespace Objects {

void drawPlane(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat x, GLfloat y, GLfloat z, GLuint texture);
void drawPlaneFlat(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat x, GLfloat y, GLfloat z, GLuint texture);
void drawFrame(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat w, GLfloat h,  GLuint texture_photo);
void drawBarrier(GLfloat x_0, GLfloat y_0, GLfloat z_0);

} // namespace Objects
} // namespace Models
