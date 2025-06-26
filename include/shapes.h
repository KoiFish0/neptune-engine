#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <stdio.h>

#include "globals.h"

// Compiler gets upset if you don't declare/define things in this order
class Triangle2D;
class ColorRect2D;
class TextureRect2D;
class TextureRect3D;
class Cube;

std::vector<Triangle2D> triangle2Ds;
std::vector<ColorRect2D> ColorRect2Ds;
std::vector<TextureRect2D> TextureRect2Ds;
std::vector<TextureRect3D> TextureRect3Ds;
std::vector<Cube> Cubes;

//
// Triangle2D
//

class Triangle2D {
private:
  unsigned int VAO, VBO;

public:
  float posX, posY;
  unsigned int shaderProgram;

  static Triangle2D create(std::vector<float> &vertices, unsigned int shaderProgram) {
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Triangle2D triangle;
    triangle.VAO = VAO;
    triangle.VBO = VBO;
    triangle.shaderProgram = shaderProgram;

    triangle2Ds.push_back(triangle);
    return triangle;
  }

  void draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
};

//
// ColorRect2D
//

// Untested, may not work
class ColorRect2D {
private:
  unsigned int VAO, VBO;

public:
  float posX, posY;
  unsigned int shaderProgram;

  static ColorRect2D create(std::vector<float> &vertices, std::vector<unsigned int> &indices, unsigned int shaderProgram) {
    unsigned int VAO, VBO, EBO;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    ColorRect2D rect;
    rect.VAO = VAO;
    rect.VBO = VBO;
    rect.shaderProgram = shaderProgram;

    ColorRect2Ds.push_back(rect);
    return rect;
  }

  void draw() {
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

class TextureRect2D {
private:
  unsigned int VAO, VBO;

public:
  float posX, posY;
  unsigned int shaderProgram;
  unsigned int texture1;
  unsigned int texture2;

  static TextureRect2D create(std::vector<float> &vertices, std::vector<unsigned int> &indices, unsigned int shaderProgram) {
    unsigned int VAO, VBO, EBO;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    TextureRect2D rect;
    rect.VAO = VAO;
    rect.VBO = VBO;
    rect.shaderProgram = shaderProgram;

    TextureRect2Ds.push_back(rect);
    return rect;
  }

  void draw() {
    glUseProgram(shaderProgram);
    if (texture1 && !texture2) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
    }

    if (texture2) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

class TextureRect3D {
private:
  unsigned int VAO, VBO;

public:
  glm::vec3 pos;
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  unsigned int shaderProgram;
  unsigned int texture1;
  unsigned int texture2;

  static TextureRect3D create(std::vector<float> &vertices, std::vector<unsigned int> &indices, unsigned int shaderProgram) {
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
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    TextureRect3D rect;
    rect.VAO = VAO;
    rect.VBO = VBO;
    rect.shaderProgram = shaderProgram;

    TextureRect3Ds.push_back(rect);
    return rect;
  }

  void draw() {
    glUseProgram(shaderProgram);

    if (texture1 && !texture2) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
    }

    if (texture2) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
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
    
    /* Matrix multiplication is non-commutative
     * so it must be multiplied in this order.
     * */

    glm::mat4 mvp = projection * view * model;

    int mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

class Cube {
private:
  unsigned int VAO, VBO;

public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  unsigned int shaderProgram;
  unsigned int texture1;
  unsigned int texture2;

  static Cube create(unsigned int shaderProgram) {
    
    // This monstrocity contains the vertex info for a cube
 
    std::vector<float> vertices = {
      // Position           // Color           // Texture Coords
      // Front face
      -0.5f, -0.5f,  0.5f,   1, 0, 0,          0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,   0, 1, 0,          1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,   0, 0, 1,          1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,   1, 1, 0,          0.0f, 1.0f,

      // Back face
      -0.5f, -0.5f, -0.5f,   1, 0, 1,          1.0f, 0.0f,
      0.5f, -0.5f, -0.5f,   0, 1, 1,          0.0f, 0.0f,
      0.5f,  0.5f, -0.5f,   1, 1, 1,          0.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

      // Left face
      -0.5f, -0.5f, -0.5f,   1, 0, 1,          0.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,   0, 1, 1,          1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,   1, 1, 1,          1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

      // Right face
      0.5f, -0.5f,  0.5f,   1, 0, 1,          0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,   0, 1, 1,          1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,   1, 1, 1,          1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

      // Top face
      -0.5f,  0.5f,  0.5f,   1, 0, 0,          0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,   0, 1, 0,          1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,   0, 0, 1,          1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,   1, 1, 0,          0.0f, 1.0f,

      // Bottom face
      -0.5f, -0.5f, -0.5f,   1, 0, 1,          0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,   0, 1, 1,          1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,   1, 1, 1,          1.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f, 0.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
      // Front face
      0, 1, 2,  2, 3, 0,
      // Back face
      4, 5, 6,  6, 7, 4,
      // Left face
      8, 9,10, 10,11, 8,
      // Right face
      12,13,14, 14,15,12,
      // Top face
      16,17,18, 18,19,16,
      // Bottom face
      20,21,22, 22,23,20
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
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    Cube cube;
    cube.VAO = VAO;
    cube.VBO = VBO;
    cube.shaderProgram = shaderProgram;

    Cubes.push_back(cube);
    return cube;
  }

  void draw() {
    glUseProgram(shaderProgram);

    if (texture1 || texture2) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
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
    
    /* Matrix multiplication is non-commutative
     * so it must be multiplied in this order.
     * */

    glm::mat4 mvp = projection * view * model;

    int mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

#endif 
