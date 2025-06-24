/* MAIN.CPP
 *
 * This is a file for testing the
 * engine as it is developed. It
 * should serve as a good sample 
 * file for the latest features.
 *
 * */

// Include the engine
#include "engine.h"

int width = 1000;
int height = 1000;

int main(void) {
  // Initialize Neptune
  Engine::initialize();

  // Create a window
  window = Engine::createWindow(width, height, "Test");

//  Engine::wireframeView(true);

  std::vector<float> vertices = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  std::vector<unsigned int> indices = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };  

  // Load and create a shader program from a file
  Shader shaderProgram("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  
  unsigned int woodTexture = Texture::loadTexture("assets/wood.jpg");

  //  Triangle2D::create(vertices, shaderProgram); 
  Rect2D::create(vertices, indices, woodTexture, shaderProgram.ID); 

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    Engine::refresh();
  }

  // Terminate the window
  Engine::terminate();
  return 0;
}

