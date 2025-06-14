#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <math.h>

#include "shader.h"
#include "primitives.h"
#include "globals.h"

double pi = 2*acos(0.0);

float aspectRatio = width / height; 

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
        vertices.push_back(0.0f); vertices.push_back(0.0f);
        vertices.push_back((cos(theta) * radius) * aspectRatio); vertices.push_back(sin(theta) * radius);
        vertices.push_back((cos(nextTheta) * radius) * aspectRatio); vertices.push_back(sin(nextTheta) * radius);
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

std::vector<Rect> rects;

Rect drawRect2D(float scaleX, float scaleY, std::string color, unsigned int shaderProgram, bool isGlobal) {
  std::vector<float> vertices = {
    // First triangle
    -0.5f * scaleX * aspectRatio, -0.5f * scaleY,
     0.5f * scaleX * aspectRatio, -0.5f * scaleY,
    -0.5f * scaleX * aspectRatio,  0.5f * scaleY,

    // Second triangle
    -0.5f * scaleX * aspectRatio,  0.5f * scaleY,
     0.5f * scaleX * aspectRatio, -0.5f * scaleY,
     0.5f * scaleX * aspectRatio,  0.5f * scaleY
  };

  // Triangle rendering
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)0);
  glEnableVertexAttribArray(0);

  Rect rect;
  rect.VAO = VAO;
  rect.VBO = VBO;
  rect.posX = 0.0f;
  rect.posY = 0.0f;
  rect.scaleX = scaleX;
  rect.scaleY = scaleY;

  if (color != "" && shaderProgram != 0) {
    std::cout << "Two shader programs provided. Defaulting to inputted shader program. Suppress this warning by setting the unused value to 0 (shaderProgram) or and empty string (color)." << std::endl;
  }

  if (color != "") {
    rect.shaderProgram = colorToShader(color);
  }

  if (shaderProgram != 0) {
    rect.shaderProgram = shaderProgram;
  }
  if (isGlobal) {
    rects.push_back(rect);
  }

  return rect;
}

