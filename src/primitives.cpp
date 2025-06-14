#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <math.h>

#include "shader.h"
#include "primitives.h"
#include "globals.h"

double pi = 2*acos(0.0);

float aspectRatio = (float)width / (float)height; 

// Array to store triangle objects
std::vector<Triangle> triangles;

/* Creates a 2D triangle and puts it in an array.
 *
 * The posX, posY arguments just specify the
 * position in NDC space that the triangle 
 * will begin with.
 *
 * Vertices is an array of type: std::vector<float>
 * and specifies the positions of each of the three
 * vertices in a triangle. 
 *
 * The shader program specifies a compiled shader 
 * program to be used when drawing the triangle.
 * */

Triangle createTriangle2D(float posX, float posY, std::vector<float> vertices, unsigned int shaderProgram) {
  // Some boilerplate code 
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);
  glEnableVertexAttribArray(0);

  Triangle triangle; // Create a Triangle object to store data about the triangle
  triangle.VAO = VAO;
  triangle.VBO = VBO;
  triangle.posX = posX;
  triangle.posY = posY;
  triangle.shaderProgram = shaderProgram;

  triangles.push_back(triangle); // Send the data to the triangles array

  return triangle;
}

// Array to store circle objects
std::vector<Circle> circles;

/* Creates a 2D circle and puts it in an array.
 * 
 * The posX, posY arguments just specify the
 * position in NDC space that the circle will 
 * begin with.
 *
 * Circles are essentially just polygons with
 * an infinite amount of faces. For performance
 * reasons, you need to specify the amount of 
 * faces. 64 or 128 is a pretty good number. 
 * This essentially specifies the detail or 
 * accuracy of the circle. The radius is well,
 * the radius of the circle in NDC space. In
 * otherwords, it's the size of the circle. The
 * shader program is the compiled shader program
 * used to render the circle.
 * */

Circle createCircle2D(float posX, float posY, int faces, float radius, unsigned int shaderProgram) {
    float deltaTheta = 2 * pi / faces;
    std::vector<float> vertices;

    for (int i = 0; i < faces; i++) {
        float theta = i * deltaTheta;
        float nextTheta = (i + 1) * deltaTheta;

        // Triangle from center to current and next edge point
        vertices.push_back(0.0f); vertices.push_back(0.0f); // Centre
        vertices.push_back((cos(theta) * radius) * aspectRatio); vertices.push_back(sin(theta) * radius); // Position of vertex at angle theta from the centre
        vertices.push_back((cos(nextTheta) * radius) * aspectRatio); vertices.push_back(sin(nextTheta) * radius); // Position of vertex at angle nextTheta (angle theta for the next iteration) from the centre
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);
    
    // Create a circle object and send relevant data to it
    Circle circle;
    circle.VAO = VAO;
    circle.VBO = VBO;
    circle.vertexCount = static_cast<int>(vertices.size() / 2);
    circle.radius = radius;
    circle.posX = 0.0f;
    circle.posY = 0.0f;
    circle.shaderProgram = shaderProgram;
    
    // Put the circle in the previously declared array
    circles.push_back(circle);
    return circle;
}

// Array to store rect objects
std::vector<Rect> rects;

/* Create a quadrilateral given an array
 * of 4 vertices as 2D floating point 
 * positions. The rest of the arguments
 * are the same as the previous methods.
 * */

Rect createRect2D(float posX, float posY, std::vector<float> vertices, unsigned int shaderProgram) {

  /* Convert vertices which expects 4 positions to
   * an array containing 6 positions which is needed
   * for the 2 triangles making up the rect.
   * */

  // Convert quad to 2 triangles (6 vertices)
  std::vector<float> triVertices = {
      // Triangle 1 (p0, p1, p2)
      vertices.data()[0], vertices.data()[1], // p0
      vertices.data()[2], vertices.data()[3], // p1
      vertices.data()[4], vertices.data()[5], // p2

      // Triangle 2 (p0, p2, p3)
      vertices.data()[0], vertices.data()[1], // p0
      vertices.data()[4], vertices.data()[5], // p2
      vertices.data()[6], vertices.data()[7]  // p3
  };

  // Triangle rendering
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, triVertices.size() * sizeof(float), triVertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);
  glEnableVertexAttribArray(0);

  // Create a rect object and send relevant data to it
  Rect rect;
  rect.VAO = VAO;
  rect.VBO = VBO;
  rect.posX = posX;
  rect.posY = posY;
  rect.shaderProgram = shaderProgram;

  // Put the rect in the previously declared array
  rects.push_back(rect);
  return rect;
}

