// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// https://github.com/g-truc/glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "model.h"

std::vector<Mesh> meshes;

Mesh createMesh3D(glm::vec3 pos, std::vector<float> vertices, unsigned int shaderProgram) {
  // Create the triangle VAO and VBO  
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  Mesh mesh; // Create a mesh object to store data about the triangle
  mesh.VAO = VAO;
  mesh.VBO = VBO;
  mesh.pos = pos;
  mesh.shaderProgram = shaderProgram;

  mesh.vertices = vertices;

  meshes.push_back(mesh); // Send the data to the meshes array

  return mesh;
}
