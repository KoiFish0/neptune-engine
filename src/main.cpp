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
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
  };

  std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };  

  //  Triangle2D::create(vertices, Shader::colorToShader2D("#ffbbff")); 
  Rect2D::create(vertices, indices, Shader::colorToShader2D("#bbbbbb")); 

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    Engine::refresh();
  }

  // Terminate the window
  Engine::terminate();
  return 0;
}

