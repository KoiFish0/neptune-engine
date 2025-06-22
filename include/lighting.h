// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// https://github.com/g-truc/glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "model.h"

struct pointLight {

  pointLight create(glm::vec3 pos, float brightness) {
    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    pointLight light;
    light.pos = pos;
    light.brightness = brightness;

    return light;
  }
};
