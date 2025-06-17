#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "globals.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

struct Mesh {
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  glm::vec3 pos;
  glm::vec3 rotation;
  glm::vec3 scale;
  unsigned int shaderProgram;
  std::vector<float> vertices;

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
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // TODO Seg Fault Here
  }  
};

Mesh createMesh3D(glm::vec3, std::vector<float>, unsigned int); 

extern std::vector<Mesh> meshes;

#endif // MODEL_H
