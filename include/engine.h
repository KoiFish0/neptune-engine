#ifndef ENGINE_H
#define ENGINE_H

// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <input.h>
#include <camera.h>
#include <globals.h>
#include <texture.h>
#include <objects.h>

GLFWwindow* window;
Camera activeCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.0f);

#define CURSOR_NORMAL     0x00034001
#define CURSOR_HIDDEN     0x00034002
#define CURSOR_DISABLED   0x00034003
#define CURSOR_CAPTURED   0x00034004

/* Rendering specifics */
#define terminate() glfwTerminate()
#define verticalSync(enabled) glfwSwapInterval(enabled)
#define wireframe(enabled) glPolygonMode(GL_FRONT_AND_BACK, (enabled) ? GL_LINE : GL_FILL)
#define cullBackFace(enabled) (enabled) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE)

/* Input modes */
#define cursorMode(mode) glfwSetInputMode(window, GLFW_CURSOR, mode);
#define stickyKeys(enabled) glfwSetInputMode(window, GLFW_STICKY_KEYS, enabled);
#define stickyMouseButtons(enabled) glfwSetInputMode(window, GLFW_STICKY_KEYS, enabled);
#define lockKeyMods(enabled) glfwSetInputMode(window, GLFW_CURSOR, enabled);
#define rawMouseMotion(enabled) glfwSetInputMode(window, GLFW_CURSOR, enabled);


class Engine {
private:
  static void draw() {
    for (Model* Model : Models) {
      Model->draw();
    }

    for (Cube* cube : Cubes) {
      cube->draw();
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
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
      while (GLenum error = glGetError()) {
        printf("Error while creating window: %u", error);
      }
      glfwTerminate();
      return window;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    /* Handle draw order */
    glEnable(GL_DEPTH_TEST);
    /* Back face culling */
    glEnable(GL_CULL_FACE);

    return window;
  }

  static void refresh() {
    Input::updateInputState(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw(); 
    glfwSwapBuffers(window);
  }
};

#endif
