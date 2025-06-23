/* MAIN.CPP
 *
 * This file  is just for development and is
 * not actually part of the engine. This 
 * should serve as a decent sample file until
 * a proper one is created for that purpose.
 *
 * */

// Include the engine
#include "engine.h"

int width = 2560;
int height = 1440;

int main(void) {
  // Initialize Neptune
  Engine::initialize();

  // Create a window
  window = Engine::createWindow(width, height, "Test");

  std::vector<float> vertices = {
    // x, y for each vertex
    0.0f, 0.5f,   // Vertex 1
    -0.5f, -0.5f, // Vertex 2
    0.5f, -0.5f   // Vertex 3
  };

  Triangle2D::create(0.0f, 0.0f, vertices, Shader::colorToShader2D("#ffbbff")); 

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    Engine::refresh();
  }
  
  // Terminate the window
  Engine::terminate();
  return 0;
}

