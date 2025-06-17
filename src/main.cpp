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
#include "iostream"
#include "fstream"

int width = 2560;
int height = 1440;

int main(void) {
  // Initialize Neptune
  engine.initialize();

  // Create a window
  window = engine.createWindow(width, height, "Test");

  float vertices[] = {
       0.0f, -0.5f, 50.0f,  // Top vertex
       50.0f, -0.5f, 0.0f,  // Bottom left
       0.5f, -0.5f, 0.0f  // Bottom right
  };

  float vertices2[] = {
       0.0f, -0.5f, 1.0f,  // Top vertex
       1.0f, 0.5f, 0.0f,  // Bottom left
       0.5f, -0.5f, 0.0f  // Bottom right
  };

  std::vector<float> vertices3 = {
       0.0f, -0.5f, 1.0f,  // Top vertex
       0.0f, -0.5f, 0.0f,  // Bottom left
       0.5f, -0.5f, 0.0f  // Bottom right
  };

  Camera camera;

  camera = camera.create(
      glm::vec3(0.0f, 0.0f, 2.0f),   
      glm::vec3(0.0f, 0.0f, -1.0f),  
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::radians(70.0f),           
      16.0f / 9.0f,                  
      0.1f,                          
      100.0f                         
  );
  
  // Set active camera
  activeCamera = camera;

  // Create a 3D triangle
  Triangle3D triangle2 = createTriangle3D(glm::vec3(0.0f, 0.0f, 0.0f), vertices2, colorToShader("#ffffff"));

  createMesh3D(glm::vec3(0.0f, 0.0f, 0.0f), vertices3, colorToShader("#ddff00"));

  // Define movement speed
  float speed = 3.0f;

  double lastX = getMouseX();
  double lastY = getMouseY();

  float sensitivity = 0.15f;

  float yaw = -90.0f; // Forward
  float pitch = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    double deltaX = getMouseX() - lastX;
    double deltaY = lastY - getMouseY();
    
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    lastX = getMouseX();
    lastY = getMouseY();

    yaw += deltaX;
    pitch += deltaY;

    // Limit pitch to avoid flipping
    if (pitch > 89.0) {
        pitch = 89.0;
    }

    if (pitch < -89.0) {
        pitch = -89.0;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    activeCamera.lookVector = normalize(front);

    if (getKeyDown(GLFW_KEY_W)) {
      activeCamera.position.z -= cos(glm::radians(yaw + 90.0f)) * speed * delta;
      activeCamera.position.x -= -sin(glm::radians(yaw + 90.0f)) * speed * delta;
    }

    if (getKeyDown(GLFW_KEY_S)) {
      activeCamera.position.z -= -cos(glm::radians(yaw + 90.0f)) * speed * delta;
      activeCamera.position.x -= sin(glm::radians(yaw + 90.0f)) * speed * delta;
    }

    if (getKeyDown(GLFW_KEY_A)) {
      activeCamera.position.z -= cos(glm::radians(yaw)) * speed * delta;
      activeCamera.position.x -= -sin(glm::radians(yaw)) * speed * delta;
    }

    if (getKeyDown(GLFW_KEY_D)) {
      activeCamera.position.z += cos(glm::radians(yaw)) * speed * delta;
      activeCamera.position.x += -sin(glm::radians(yaw)) * speed * delta;
    }

    if (getKeyDown(GLFW_KEY_SPACE)) {
      activeCamera.position.y += speed * delta;
    }

    if (getKeyDown(GLFW_KEY_LEFT_SHIFT)) {
      activeCamera.position.y -= speed * delta;
    }

    engine.refresh();
  }
  
  // Terminate the window
  engine.terminate();
  return 0;
}

