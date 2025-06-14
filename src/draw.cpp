#include "primitives.h"
#include "draw.h"

void drawElements() {
    for (unsigned int vao : triangleVAOs) {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    for (Triangle& triangle : triangles) {
      triangle.draw(triangle.shaderProgram);
    }

    for (Rect& rect : rects) {
      rect.draw(rect.shaderProgram);
    }

    for (Circle& circle : circles) {
      circle.draw(circle.shaderProgram);
    }
}
