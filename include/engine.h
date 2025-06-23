#ifndef ENGINE_H
#define ENGINE_H

// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shapes.h"
#include "shader.h"
#include "input.h"
#include "camera.h"
#include "globals.h"

class Engine {
private:

  static void draw() {
    for (Triangle2D& triangle2D : triangle2Ds) {
      triangle2D.draw();
    }
  }

public:
  static int initialize() {
    // Initialize GLFW
    if (!glfwInit())
      return -1;

    // Initialize input 
    Input::inputInit();
    return 0;
  }

  static GLFWwindow* createWindow(int width, int height, const char *title) {
    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
      glfwTerminate();
      printf("Failed to create window.");
      // Return a handle to the window
      return window;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    // Load OpenGL
    gladLoadGL();
    
    // Handle draw order
    glEnable(GL_DEPTH_TEST);

    // Capture the mouse and hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
  }

  static void refresh() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Register user inputs 
    Input::updateInputState(window);
    
    // Custom draw function for drawing primitives
    draw(); 
    
    // Swap front and back buffers
    glfwSwapBuffers(window);
  }

  static void terminate() {
    glfwTerminate();
  }
};

// A pointer to the window; gives a handle to the window to other files
GLFWwindow* window;

// Current active camera; allows multiple cameras and swapping between them if needed 
Camera activeCamera;

#endif // ENGINE_H
