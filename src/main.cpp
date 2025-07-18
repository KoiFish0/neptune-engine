/* MAIN.CPP
 *
 * A little testing file. Somewhat of a sample file.
 *
 * */

// Include the engine
#include "engine.h"

int width = 2560;
int height = 1440;

int main(void) {
  Engine::initialize();

  Engine::createWindow(width, height, "Test");

  // Background
  glClearColor(0.25f * 0.5f, 0.25f * 0.5f, 0.35f * 0.5f, 1.0f);

//  Engine::wireframeView(true);

  // Load and create a shader program from a file
  Shader shaderProgram("src/shaders/presets/phongTextureSpecMap.vert", "src/shaders/presets/phongTextureSpecMap.frag");

  unsigned int diffuse = Texture::loadTextureRGBA("assets/container2.png", 1);
  unsigned int specMap = Texture::loadTextureRGBA("assets/container2_specular.png", 0);

  Cube cube = Cube::create(shaderProgram.ID);
  Cube cube2 = Cube::create(shaderProgram.ID);

  // Assign textures to be used in the draw call of the corresponding cube
  Cubes[0].texture1 = diffuse;
  Cubes[1].texture1 = diffuse;
  Cubes[0].texture2 = specMap;
  Cubes[1].texture2 = specMap;

  Cubes[1].pos.x = 1.5f;
  Cubes[1].pos.z = -1.5f;
  
  glm::vec3 lightPos = glm::vec3(3.0f, 1.0f, 1.5f);

  // Set active shader before modifying uniforms
  // TODO Handle it under the hood
  shaderProgram.use();

  // Set texture uniform
  shaderProgram.setInt("material.diffuse", 1);
  shaderProgram.setInt("material.specular", 0);

  shaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
//  shaderProgram.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
//  shaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shaderProgram.setFloat("material.shininess", 32.0f);

  shaderProgram.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
  shaderProgram.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
  shaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 

  shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shaderProgram.setVec3("lightColor",  0.8f, 0.8f, 0.8f);
  shaderProgram.setVec3("lightPos", lightPos);

  Camera camera;

  camera = camera.create(
      glm::vec3(0.0f, 0.0f, 2.0f),   
      glm::vec3(0.0f, 0.0f, -1.0f),  
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::radians(70.0f),           
      16.0f / 9.0f,                  
      0.025f,                          
      100.0f                         
  );
  
  // Set active camera
  activeCamera = camera;

  // Define movement speed
  float speed = 3.0f;

  double lastX = Input::getMouseX();
  double lastY = Input::getMouseY();

  float sensitivity = 0.15f;

  float yaw = -90.0f; // Forward
  float pitch = 0.0f;

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    shaderProgram.setVec3("viewPos", activeCamera.position); 
    double deltaX = Input::getMouseX() - lastX;
    double deltaY = lastY - Input::getMouseY();
    
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    lastX = Input::getMouseX();
    lastY = Input::getMouseY();

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

    if (Input::getKeyDown(GLFW_KEY_W)) {
      activeCamera.position.z -= cos(glm::radians(yaw + 90.0f)) * speed * delta;
      activeCamera.position.x -= -sin(glm::radians(yaw + 90.0f)) * speed * delta;
    }

    if (Input::getKeyDown(GLFW_KEY_S)) {
      activeCamera.position.z -= -cos(glm::radians(yaw + 90.0f)) * speed * delta;
      activeCamera.position.x -= sin(glm::radians(yaw + 90.0f)) * speed * delta;
    }

    if (Input::getKeyDown(GLFW_KEY_A)) {
      activeCamera.position.z -= cos(glm::radians(yaw)) * speed * delta;
      activeCamera.position.x -= -sin(glm::radians(yaw)) * speed * delta;
    }

    if (Input::getKeyDown(GLFW_KEY_D)) {
      activeCamera.position.z += cos(glm::radians(yaw)) * speed * delta;
      activeCamera.position.x += -sin(glm::radians(yaw)) * speed * delta;
    }

    if (Input::getKeyDown(GLFW_KEY_SPACE)) {
      activeCamera.position.y += speed * delta;
    }

    if (Input::getKeyDown(GLFW_KEY_LEFT_SHIFT)) {
      activeCamera.position.y -= speed * delta;
    }

    // FPS
    //printf("%f\n", 1 / delta);

    Engine::refresh();
  }

  // Terminate the window
  Engine::terminate();
  return 0;
}

