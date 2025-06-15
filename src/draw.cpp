#include "primitives.h"
#include "draw.h"

void drawElements() {
    for (Triangle2D& triangle2D : triangles2D) {
      triangle2D.draw(triangle2D.shaderProgram);
    }

    for (Rect2D& rect2D : rects2D) {
      rect2D.draw(rect2D.shaderProgram);
    }

    for (Circle2D& circle2D : circles2D) {
      circle2D.draw(circle2D.shaderProgram);
    }
 
    // 3D 
    for (Triangle3D& triangle3D : triangles3D) {
      triangle3D.draw(triangle3D.shaderProgram);
    }
}
