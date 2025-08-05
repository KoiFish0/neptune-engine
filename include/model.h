#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "globals.h"

class Mesh;

std::vector<Mesh*> Meshes;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
private:
  unsigned int VAO, VBO, EBO;

public:
  // Mesh data
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  unsigned int shaderProgram;
  std::vector<unsigned int> textures;
  std::vector<unsigned int> indices;

  static Mesh* create(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int shaderProgram) {
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

    glBindVertexArray(0);

    Mesh* mesh = new Mesh;
    mesh->VAO = VAO;
    mesh->VBO = VBO;
    mesh->VBO = EBO;
    mesh->indices = indices;
    mesh->shaderProgram = shaderProgram;

    Meshes.push_back(mesh);
    return mesh;
  }

  void draw() {
    glUseProgram(shaderProgram);

    for (int i = 0; i < textures.size(); ++i) {
      glActiveTexture(GL_TEXTURE1 + textures[i]);
      glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

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
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};  

class Model {
public:
  Model(char *path) {
    loadModel(path);
  }

  void Draw(unsigned int shaderProgram) {
    for(unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].draw();
  }  

private:
  // model data
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  }

  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<unsigned int> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
