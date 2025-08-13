/*
 * include/objects.h
 *
 * Handle creation and management of meshes, models and some pre-made 3D Objects.
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include "shader.h"
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

#include <iostream>
#include <string>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh;

std::vector<Mesh*> Meshes;

class Mesh {
private:

public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  std::vector<unsigned int> textures;
  std::vector<unsigned int> indices;
  unsigned int shaderProgram;
  unsigned int VAO, VBO, EBO;

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


/* WIP This class is currently very unfinished */
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
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
      return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
  }  

  void processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
      meshes.push_back(processMesh(mesh, scene));			
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }  

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      float vertex;
      // process vertex positions, normals and texture coordinates
      vertices.push_back(vertex);
    }
    // process indices
    //
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
      // [...]
    }

    return *Mesh::create(vertices, indices, 0);
  }  

  std::vector<unsigned int> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

/* 3D Objects */
class Cube;

std::vector<Cube*> Cubes;

class Cube {
private:
  unsigned int VAO, VBO;
  Mesh* mesh;

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

    /* The mesh must outlive the cube object to avoid invalid references */
    Mesh* cubeMesh = Mesh::create(vertices, indices, shaderProgram);
    /* TODO Create destructor for cube */
    Cube* cube = new Cube;

    if (cubeMesh == nullptr) {
      printf("Failed to initialize mesh while creating cube.");
      return cube;
    }

    cube->VAO = cubeMesh->VAO;
    cube->VBO = cubeMesh->VBO;
    cube->mesh = cubeMesh;
    cube->shaderProgram = cubeMesh->shaderProgram;

    Cubes.push_back(cube);
    return cube;
  }

  /*
   * Drawing of the cube is handled by the mesh attached to the cube object.
   * That draw call uses the transformations of the mesh to transform the model
   * matrix. The methods below expose transformations of the cube.
   */
  void setPos(float x, float y, float z) {
    mesh->pos = glm::vec3(x, y, z);
  }

  void setScale(float x, float y, float z) {
    mesh->scale = glm::vec3(x, y, z);
  }

  void setRotation(float x, float y, float z) {
    mesh->rotation = glm::vec3(x, y, z);
  }
};

class SubdividedPlane;

std::vector<SubdividedPlane*> SubdividedPlanes;

class SubdividedPlane {
private:
  unsigned int VAO, VBO;

public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  unsigned int shaderProgram;
  std::vector<unsigned int> textures;
  std::vector<unsigned int> indices;

  static SubdividedPlane* create(int subdivisions, unsigned int shaderProgram) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const float planeWidth = 10.0f;
    const float planeHeight = 10.0f;

    /* Vertices */
    for (unsigned i = 0; i <= subdivisions; i++) {
      for (unsigned j = 0; j <= subdivisions; j++) {
        // Vertex 0: Bottom-left
        vertices.push_back(-planeWidth / 2.0f + planeWidth * i / (float)subdivisions); // v.x
        vertices.push_back(0.0f); // v.y
        vertices.push_back(-planeHeight / 2.0f + planeHeight * j / (float)subdivisions); // v.z
        vertices.push_back(i / (float)subdivisions); // u
        vertices.push_back(j / (float)subdivisions); // v
      }
    }

    /* Indices */
    for (unsigned i = 0; i < subdivisions; i++) {
      for (unsigned j = 0; j < subdivisions; j++) {
        unsigned baseIndex = i * (subdivisions + 1) + j;

        indices.push_back(baseIndex); /* bottom-left */
        indices.push_back(baseIndex + 1); /* bottom-right */
        indices.push_back(baseIndex + (subdivisions + 1)); /* top-left */

        indices.push_back(baseIndex + 1); /* bottom-right */
        indices.push_back(baseIndex + (subdivisions + 1) + 1); /* top-right */
        indices.push_back(baseIndex + (subdivisions + 1)); /* top-left */
      }
    }

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    SubdividedPlane* subdividedPlane = new SubdividedPlane;
    subdividedPlane->VAO = VAO;
    subdividedPlane->VBO = VBO;
    subdividedPlane->indices = indices;
    subdividedPlane->shaderProgram = shaderProgram;

    SubdividedPlanes.push_back(subdividedPlane);
    return subdividedPlane;
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

#endif
