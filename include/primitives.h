#ifndef PRIMITIVES_H
#define PRIMITIVES_H
/* PRIMITIVES.H
 *
 * Handles classes and draw calls for primitive
 * shapes (triangles, rects, circles). Stores
 * data for multiple primitive objects in an
 * array (std::vector<{primitive}>).
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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "globals.h"

// 2D

/* Triangle class 
 * 
 * - Stores data for a given triangle
 * - Includes a draw method to draw the triangle
 * */

struct Triangle2D {
  // Triangle data
  unsigned int VAO;
  unsigned int VBO;
  float posX, posY;
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

struct Circle2D {
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

struct Rect2D {
  unsigned int VAO;
  unsigned int VBO;
  float posX, posY;
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
extern std::vector<Triangle2D> triangles2D;
extern std::vector<Circle2D> circles2D;
extern std::vector<Rect2D> rects2D;

// Declare some functions for creating/initializing primitives
Triangle2D createTriangle2D(float, float, std::vector<float>, unsigned int); 

Circle2D createCircle2D(int, float, std::string);

Rect2D createRect2D(float, float, std::vector<float>, unsigned int);

// 3D

/* Triangle3D class
 *
 * Stores data for a 3D triangle object
 * and includes a draw call for rendering
 * each frame.
 * */

struct Triangle3D {
  unsigned int VAO;
  unsigned int VBO;
  glm::vec3 pos;
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  unsigned int shaderProgram;

  /* This draw method is mostly similar to the
   * 2D counterpart, however, it uses a proper
   * Model, View, Projection (MVP) matrix for
   * 3D rendering. Model represents the 
   * transformations of the triangle. View
   * represents the view matrix of the active
   * camera / viewport. Projection is for
   * taking into account perspective, or in 
   * other words, how things farther away 
   * appear smaller. Multiplying these values
   * together gives us the MVP matrix which we
   * can then hand over to the shader to 
   * create the illusion of a 3D triangle.
   * */

  void draw(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    // Build the transformation matrix
    glm::mat4 identity = glm::mat4(1.0f); // identity matrix 
    
    // Apply translation
    glm::mat4 model = glm::translate(identity, pos); 

    // Apply rotation on top of translation
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0)); 
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0)); 
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1)); 
    
    // TODO Look into quaternions; something like this
//    glm::quat q = glm::quat(rotation);        // convert euler angles (XYZ) to quaternion
//    glm::mat4 rotMatrix = glm::toMat4(q);     // get rotation matrix from quaternion
//    model *= rotMatrix;

    // Apply scale on top of translation and rotation
    model = glm::scale(model, scale); 
    
    // Create the view matrix
    glm::mat4 view = activeCamera.getViewMatrix();
    
    // Create a perspective projection matrix
    glm::mat4 projection = activeCamera.getProjectionMatrix();
    
    /* Matrix multiplication is non-commutative
     * so it must be multiplied in this order.
     * */

    glm::mat4 mvp = projection * view * model;

    // Send the transform to the shader
    int transformLoc = glGetUniformLocation(shaderProgram, "u_transform");

    glUniformMatrix4fv(transformLoc, 1, 0, glm::value_ptr(mvp));

    // Bind VAO and draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }  

};

extern std::vector<Triangle3D> triangles3D;

Triangle3D createTriangle3D(glm::vec3, float[], unsigned int);

#endif // PRIMITIVES_H
