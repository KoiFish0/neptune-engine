#ifndef ENGINE_H
#define ENGINE_H

// Include OpenGL wrapper libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "primitives.h"
#include "input.h"
#include "draw.h"
#include "camera.h"
#include "model.h"

struct Engine {

  int initialize() {
    // Initialize GLFW
    if (!glfwInit())
      return -1;

    // Initialize input 
    inputInit();
    return 0;
  }

  GLFWwindow* createWindow(int width, int height, const char *title) {
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

  void refresh() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Register user inputs 
    updateInputState(window);
    
    // Custom draw function for drawing primitives
    drawElements(); 
    
    // Swap front and back buffers
    glfwSwapBuffers(window);
  }

  void terminate() {
    glfwTerminate();
  }
};

// A pointer to the window; gives a handle to the window to other files
GLFWwindow* window;

// Current active camera; allows multiple cameras and swapping between them
Camera activeCamera;
// A handle the engine
Engine engine;

#endif // ENGINE_H
