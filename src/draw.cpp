/* draw.cpp
 *
 * This file  allows for rendering of
 * all primitives by iterating through
 * their corresponding list.
 * */

#include "primitives.h"
#include "model.h"
#include "draw.h"

void drawElements() {
    // 2D
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
    
    for (Mesh& mesh : meshes) {
    mesh.draw(mesh.shaderProgram);
  }
}
