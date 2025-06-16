/* MAIN.CPP
 *
 * This file  is just for development and is
 * not actually part of the engine. This 
 * should serve as a decent sample file until
 * a proper one is created for that purpose.
 *
 * This file creates a window and draws some 
 * shapes on the screen.
 * */

// Include the engine
#include "engine.h"

int width = 2560;
int height = 1440;

int main(void) {
  // Initialize Neptune
  engine.initialize();

  // Create a window
  window = engine.createWindow(width, height, "Test");

  std::vector<float> vertices = {
       0.0f,  0.5f, 0.0f,  // Top vertex
      -0.5f, -0.5f, 0.0f,  // Bottom left
       0.5f, -0.5f, 0.0f  // Bottom right
  };

  Camera camera;

  camera = camera.create(
      glm::vec3(0.0f, 0.0f, 2.0f),   
      glm::vec3(0.0f, 0.0f, -1.0f),  
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::radians(45.0f),           
      16.0f / 9.0f,                  
      0.1f,                          
      100.0f                         
  );
  
  // Set active camera
  activeCamera = camera;

  // Create a 3D triangle
  Triangle3D triangle = createTriangle3D(glm::vec3(0.0f, 0.0f, 0.0f), vertices, colorToShader("#ffffff"));
  
  // Define movement speed
  float speed = 1.0f;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {

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

    engine.refresh();
  }
  
  // Terminate the window
  engine.terminate();
  return 0;
}

