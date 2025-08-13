#ifndef ENGINE_H
#define ENGINE_H

// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "input.h"
#include "camera.h"
#include "globals.h"
#include "texture.h"
#include "objects.h"

GLFWwindow* window;
Camera activeCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.0f);

#define terminate() glfwTerminate()

class Engine {
private:
  static void draw() {
    for (Mesh* Mesh : Meshes) {
      Mesh->draw();
    }

    for (SubdividedPlane* subdividedPlane : SubdividedPlanes) {
      subdividedPlane->draw();
    }
  }

public:
  static int initialize() {
    /* Initialize GLFW */
    if (!glfwInit()) {
      return -1;
    }

    /* Initialize input */
    Input::inputInit();
    return 0;
  }

  static GLFWwindow* createWindow(int width, int height, const char* title) {
    /** 
     * Create a windowed mode window and its OpenGL context 
     * TODO Let the caller control window mode 
     */
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
      while (GLenum error = glGetError()) {
        std::cout << "Error while creating window: " << error << std::endl;
      }
      glfwTerminate();
      return window;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    /* Handle draw order */
    glEnable(GL_DEPTH_TEST);

    /** 
     * Capture the mouse and hide the cursor
     * TODO Create an interface for setting these parameters at runtime
     */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Enable vertical sync */
    glfwSwapInterval(1);

    return window;
  }

  static void refresh() {
    Input::updateInputState(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw(); 
    glfwSwapBuffers(window);
  }

  static void wireframeView(bool enabled) {
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
  }
};

#endif // ENGINE_H
