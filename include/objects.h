/*
 * include/objects.h
 *
 * Handle creation and management of meshes, models and some pre-made 3D Objects.
 */

#ifndef OBJECTS_H
#define OBJECTS_H

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

#include <vector>

#include <texture.h>
#include <globals.h>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};


/*
 * Define what vertex attributes are used and in what order. If a normal or
 * texture vector/coordinate isn't needed, there is no point in storing that
 * data
 */
enum VertexAttributes {
  POSITION,
  POSITION_NORMAL,
  POSITION_TEXTURE,
  POSITION_NORMAL_TEXTURE,
};

/* Vertex and index data for a given object */
struct VertexDataObject {
  unsigned int VAO, VBO, EBO;

  VertexDataObject(std::vector<float> vertices, std::vector<unsigned int> indices, enum VertexAttributes vertexAttributes) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    /* Set vertex attributes according to the attributes specified by the caller */

    /* Only position attribute */
    if (vertexAttributes == POSITION) {
      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
    } 

    /* Position and normal attributes */
    else if (vertexAttributes == POSITION_NORMAL) {
      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      // normal attribute
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
      glEnableVertexAttribArray(1);
    } 

    /* Position and texture attributes */
    else if (vertexAttributes == POSITION_TEXTURE) {
      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      // texture attribute
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);  
    } 

    /* Position, normal and texture attributes */
    else if (vertexAttributes == POSITION_NORMAL_TEXTURE) {
      // position attribute
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      // normal attribute
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
      glEnableVertexAttribArray(1);
      // texture attribute
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);  
    }
  }
};

class Mesh;

std::vector<Mesh*> Meshes;

class Mesh {
private:

public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  std::vector<Texture> textures;
  unsigned int indicesCount;
  unsigned int shaderProgram;
  unsigned int VAO, VBO, EBO;


  Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int shader) {

    VertexDataObject VDO(vertices, indices, POSITION_NORMAL_TEXTURE);

    VAO = VDO.VAO;
    VBO = VDO.VBO;
    VBO = VDO.EBO;
    indicesCount = indices.size();
    shaderProgram = shader;
  }

  void draw() {
    glUseProgram(shaderProgram);

    for (int i = 0; i < textures.size(); ++i) {
      glActiveTexture(GL_TEXTURE1 + textures[i].texture);
      glBindTexture(GL_TEXTURE_2D, textures[i].texture);
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
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};  

/* 
 * 3D Objects 
 */
class Cube;

std::vector<Cube*> Cubes;

class Cube {
public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  std::vector<unsigned int> textures;
  unsigned int shaderProgram;
  unsigned int VAO, VBO;

  Cube(unsigned int shader) {
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
      0, 3, 2,  2, 1, 0,
      // Front face (+Z)
      6, 7, 4,  4, 5, 6,
      // Left face (-X)
      10, 11, 8,  8, 9, 10,
      // Right face (+X)
      14, 15, 12,  12, 13, 14,
      // Bottom face (-Y)
      18, 19, 16,  16, 17, 18,
      // Top face (+Y)
      20, 23, 22,  22, 21, 20
    };

    VertexDataObject VDO(vertices, indices, POSITION_NORMAL_TEXTURE);

    VAO = VDO.VAO;
    VBO = VDO.VBO;
    shaderProgram = shader;

    Cubes.push_back(this);
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
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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
  std::vector<unsigned int> textures;
  unsigned int shaderProgram;
  unsigned int indicesCount;

  SubdividedPlane(int subdivisions, unsigned int shader) {
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

    VertexDataObject VDO(vertices, indices, POSITION_TEXTURE);

    VAO = VDO.VAO;
    VBO = VDO.VBO;
    indicesCount = indices.size();
    shaderProgram = shader;

    SubdividedPlanes.push_back(this);
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
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

class Model;

std::vector<Model> Models;

/*
 * Handle loading and drawing of imported 3D models. Model importing/loading is done through assimp:
 * https://github.com/assimp/assimp
 *
 * Supported formats are listed here:
 * https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
 *
 * Some code in this class is a modified version of code from:
 * https://learnopengl.com/Model-Loading/Model
 */
class Model {
public:
  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

  Model(char* path) {
    directory = path;
    loadModel(path);
  }

  void draw(unsigned int shaderProgram) {
    for (int i = 0; i < meshes.size(); ++i) {
      meshes[i].draw(shader);
    }
  }

private:
  std::vector<Mesh> meshes;
  char* directory;  
  unsigned int shader; 

  void loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	

    /* Error while importing model */
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
      return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
  }  

  void processNode(aiNode* node, const aiScene* scene) {
    /* Process all the node's meshes (if any) */
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
      meshes.push_back(processMesh(mesh, scene));			
    }

    /* Then do the same for each of its children */
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }  

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    /* Process vertices */
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vector; 

      /* Position */
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z; 
      vertex.Position = vector;
      /* Normal */
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;  

      if(mesh->mTextureCoords[0]) { /* Check if the mesh contains texture coordinates */
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x; 
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
      } else {
        /* No texture coordinates */
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }
    }

    /* 
     * Process indices 
     * For each face, retrieve its indices and push it back to the indices array
     */
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for(unsigned int j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }  

    /* Process material */

    if(mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
      vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }  

    return Mesh(vertices, indices, textures);
  }  
};

#endif
