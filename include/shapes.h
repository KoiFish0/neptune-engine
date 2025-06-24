#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

#include <stdio.h>

// Compiler gets upset if you don't declare/define things in this order
class Triangle2D;
class Rect2D;

std::vector<Triangle2D> triangle2Ds;
std::vector<Rect2D> Rect2Ds;

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

class Rect2D {
private:
  unsigned int VAO, VBO;

public:
  float posX, posY;
  unsigned int shaderProgram;
  unsigned int texture;

  static Rect2D create(std::vector<float> &vertices, std::vector<unsigned int> &indices, unsigned int newTexture, unsigned int shaderProgram) {
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

    Rect2D rect;
    rect.VAO = VAO;
    rect.VBO = VBO;
    rect.shaderProgram = shaderProgram;
    if (newTexture != 0) {
      rect.texture = newTexture;
    }

    Rect2Ds.push_back(rect);
    return rect;
  }

  void draw() {
    glUseProgram(shaderProgram);
    if (texture) {
      glBindTexture(GL_TEXTURE_2D, texture);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};


#endif 

