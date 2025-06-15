/* MAIN.CPP
 *
 * This file  is just for development and is
 * not actually part of the engine. This 
 * should serve as a decent sample file until
 * a proper one is created for that purpose.
 *
 * This file creates a window and draws some 
 * shapes on the screen.
 *
 * Some of the code in this file is from:
 * https://www.glfw.org/documentation
 *
 * It is not part of the engine but rather 
 * just some boilerplate code for opening a 
 * GLFW window.
 * */

// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Include the engine
#include "shader.h"
#include "primitives.h"
#include "globals.h"
#include "input.h"
#include "draw.h"
#include "camera.h"

int width = 2560;
int height = 1440;
GLFWwindow *globalWindow;

Camera activeCamera;

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
}

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(width, height, "Test", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  globalWindow = window;
 
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enables VSync
  gladLoadGL();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // Register resize callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Set initial viewport
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  inputInit();
  
  std::vector<float> vertices = {
       0.0f,  0.5f, 0.0f,  // Top vertex
      -0.5f, -0.5f, 0.0f,  // Bottom left
       0.5f, -0.5f, 0.0f  // Bottom right
  };

  std::vector<float> quadVertices = {
      // p0
      -0.5f,  0.5f,
      // p1
       0.5f,  0.5f,
      // p2
       0.5f, -0.5f,
      // p3
      -0.5f, -0.5f
  };
  
  // createRect2D(0.0f, 0.0f, quadVertices, colorToShader("#ffffff"));
 
  Camera camera;

  camera = camera.create(
      glm::vec3(0.0f, 0.0f, 1.0f),   // position: 5 units away from origin along z-axis
      glm::vec3(0.0f, 0.0f, -1.0f),  // lookVector: looking toward negative z-axis
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::radians(45.0f),           // fov: 45 degrees field of view in radians
      16.0f / 9.0f,                  // aspectRatio: widescreen
      0.1f,                          // nearPlane
      100.0f                         // farPlane
  );

  activeCamera = camera;

  Triangle3D triangle = createTriangle3D(glm::vec3(0.0f, 0.0f, 0.0f), vertices, colorToShader("#ffffff"));

  float speed = 1.0f;
  
  glEnable(GL_DEPTH_TEST);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Poll for and process events */
    updateInputState(window);

    if (getKeyDown(GLFW_KEY_W)) {
      activeCamera.position.z -= speed * delta;
    }

    if (getKeyDown(GLFW_KEY_S)) {
      activeCamera.position.z += speed * delta;
    }

    if (getKeyDown(GLFW_KEY_A)) {
      activeCamera.position.x -= speed * delta;
    }

    if (getKeyDown(GLFW_KEY_D)) {
      activeCamera.position.x += speed * delta;
    }

    printf("%f, %f, %f\n", activeCamera.position.x, activeCamera.position.y, activeCamera.position.z);

    drawElements(); // Custom draw function for drawing primitives
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

