#pragma once

#include <GL/gl.h>

enum Orientation {
    VERTICAL,
    HORIZONTAL,
};

enum CornerType{
    BOTTOM_LEFT = 90,
    BOTTOM_RIGHT = 180,
    UPPER_LEFT = 0,
    UPPER_RIGHT = 270,
};

namespace Models {
namespace Scene {
void drawHallway(GLfloat x_0, GLfloat y_0, GLfloat z_0, GLfloat w, GLfloat h, Orientation orientation,GLuint texture_id);
void drawCorner(GLfloat x_0,GLfloat y_0, GLfloat z_0, GLfloat w, GLfloat h, CornerType corner_type = UPPER_LEFT);

} // namespace Scene
} // namespace Models
