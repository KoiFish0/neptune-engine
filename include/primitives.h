/* PRIMITIVES.H
 *
 * Handles classes and draw calls for primitive
 * shapes (triangles, rects, circles). Stores
 * data for multiple primitive objects in an
 * array (std::vector<{primitive}).
 *
 * This file also declares functions to 
 * initialize primitives for use by external
 * files.
 * 
 * Dependencies:
 * GLAD - https://glad.dav1d.de/
 * GLFW - https://www.glfw.org/
 * GLM  - https://github.com/g-truc/glm
 * */

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "globals.h"

extern std::vector<unsigned int> triangleVAOs;

void drawTriangle2D(float*, size_t);

/* Triangle class 
 * 
 * - Stores data for a given triangle
 * - Includes a draw method to draw the triangle
 * */

struct Triangle {
  // Triangle data
  unsigned int VAO;
  unsigned int VBO;
  float posX, posY;
  float scaleX, scaleY;
  unsigned int shaderProgram;

  /*Triangle draw method
   *
   * Draws a triangle given a shader program
   * and uses the stored VAO and VBO associated
   * with the triangle object.
   *
   * Example: 
   *
   * while (gameloop) {
   *   triangle.draw();
   * }
   * */

  void draw(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    // Build the transformation matrix
    glm::mat4 transform = glm::mat4(1.0f); // identity matrix 

    // Apply translation
    transform = glm::translate(transform, glm::vec3(posX, posY, 0.0f));

    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    glm::mat4 mvp = projection * transform;

    // Send the transform to the shader
    int transformLoc = glGetUniformLocation(shaderProgram, "u_transform");

    glUniformMatrix4fv(transformLoc, 1, 0, glm::value_ptr(mvp));

    // Bind VAO and draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }  
};

/* Circle class 
 * 
 * - Stores data for a given circle 
 * - Includes a draw method to draw the circle 
 * */

struct Circle {
  unsigned int VAO;
  unsigned int VBO;
  int vertexCount;
  float radius;
  float posX, posY; 
  unsigned int shaderProgram;

  /* Circle draw method
   *
   * Draws a circle given a shader program
   * and uses the stored VAO and VBO associated
   * with the circle object.
   *
   * Example: 
   *
   * while (gameloop) {
   *   circle.draw();
   * }
   * */

  void draw(unsigned int shaderProgram) {
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    glUseProgram(shaderProgram);

    // Build the transformation matrix
    glm::mat4 transform = glm::mat4(1.0f); // identity matrix 

    // Apply translation
    transform = glm::translate(transform, glm::vec3(posX, posY, 0.0f));
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    glm::mat4 mvp = projection * transform;

    // Send the transform to the shader
    int transformLoc = glGetUniformLocation(shaderProgram, "u_transform");
    glUniformMatrix4fv(transformLoc, 1, 0, glm::value_ptr(mvp));

    // Bind VAO and draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
  }
};

/* Circle class 
 * 
 * - Stores data for a given circle 
 * - Includes a draw method to draw the circle 
 * */

struct Rect {
  unsigned int VAO;
  unsigned int VBO;
  float posX, posY;
  float scaleX, scaleY;
  unsigned int shaderProgram;

  /* Rect draw method
   * 
   * For this explanation, a rect is
   * basically just a box/rectangle.
   *
   * Draws a rect given a shader program
   * and uses the stored VAO and VBO associated
   * with the rect object.
   *
   * Example: 
   *
   * while (gameloop) {
   *   rect.draw();
   * }
   * */

  void draw(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    // Build the transformation matrix
    glm::mat4 transform = glm::mat4(1.0f); // identity matrix 

    // Apply translation
    transform = glm::translate(transform, glm::vec3(posX, posY, 0.0f));

    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    glm::mat4 mvp = projection * transform;

    // Send the transform to the shader
    int transformLoc = glGetUniformLocation(shaderProgram, "u_transform");

    glUniformMatrix4fv(transformLoc, 1, 0, glm::value_ptr(mvp));

    // Bind VAO and draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }  
};

// Some arrays to store primitive objects
extern std::vector<Triangle> triangles;
extern std::vector<Circle> circles;
extern std::vector<Rect> rects;

// Declare some functions for creating/initializing primitives
Triangle createTriangle2D(float, float, std::vector<float>, unsigned int); 

Circle createCircle2D(int, float, std::string);

Rect drawRect2D(float, float, std::string, unsigned int, bool);

