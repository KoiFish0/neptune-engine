#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "globals.h"

class Cube;

std::vector<Cube*> Cubes;

class Cube {
private:
  unsigned int VAO, VBO;

public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  unsigned int shaderProgram;
  std::vector<unsigned int> textures;

  static Cube* create(unsigned int shaderProgram) {

    std::vector<float> vertices = {
      // Back face (-Z)
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // 0
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, // 1
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // 2
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, // 3

      // Front face (+Z)
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // 4
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, // 5
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // 6
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, // 7

      // Left face (-X)
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // 8
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // 9
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 10
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 11

      // Right face (+X)
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // 12
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // 13
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 14
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 15

      // Bottom face (-Y)
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // 16
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, // 17
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // 18
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, // 19

      // Top face (+Y)
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // 20
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, // 21
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // 22
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f  // 23
    };

    std::vector<unsigned int> indices = {
      // Back face (-Z)
      0, 1, 2,  2, 3, 0,
      // Front face (+Z)
      4, 5, 6,  6, 7, 4,
      // Left face (-X)
      8, 9, 10,  10, 11, 8,
      // Right face (+X)
      12, 13, 14,  14, 15, 12,
      // Bottom face (-Y)
      16, 17, 18,  18, 19, 16,
      // Top face (+Y)
      20, 21, 22,  22, 23, 20
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    Cube* cube = new Cube;
    cube->VAO = VAO;
    cube->VBO = VBO;
    cube->shaderProgram = shaderProgram;

    Cubes.push_back(cube);
    return cube;
  }

  void draw() {
    glUseProgram(shaderProgram);

    for (int i = 0; i < textures.size(); ++i) {
      glActiveTexture(GL_TEXTURE1 + textures[i]);
      glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

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

    // Send the matrices to the shader
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

#endif 
