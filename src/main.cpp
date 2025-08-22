/* A file used for testing the engine */

#include "engine.h"

int width = 2560;
int height = 1440;

int main(void) {
  Engine::initialize();
  debugPrint = true;

  Engine::createWindow(width, height, "Test");
  verticalSync(1);
  wireframe(0);
  cursorMode(CURSOR_DISABLED);

  glClearColor(0.3f, 0.3f, 0.3f, 0.3f);

  Shader flatShader("src/shaders/terrain.vert", "src/shaders/terrain.frag");

  Cube cube(flatShader.ID);

  //Model model("assets/teapot.obj");

  cube.pos.y = 1.0f;

  SubdividedPlane plane(500, flatShader.ID);

  float planeScale = 5.0f;
  plane.scale = glm::vec3(planeScale, planeScale, planeScale);

  Camera camera(
      glm::vec3(0.0f, 0.0f, 2.0f),   
      glm::vec3(0.0f, 0.0f, -1.0f),  
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::radians(70.0f),           
      16.0f / 9.0f,                  
      0.025f,                          
      100.0f                         
  );
  
  activeCamera = camera;

  float speed = 7.0f;

  double lastX = Input::getMouseX();
  double lastY = Input::getMouseY();

  float sensitivity = 0.15f;

  /* Initial look vector */
  float yaw = -90.0f;
  float pitch = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    double deltaX = Input::getMouseX() - lastX;
    double deltaY = lastY - Input::getMouseY();

    deltaX *= sensitivity;
    deltaY *= sensitivity;

    lastX = Input::getMouseX();
    lastY = Input::getMouseY();

    yaw += deltaX;
    pitch += deltaY;

    /* Limit pitch to avoid flipping */
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

//    printf("%f\n", 1 / delta);

    Engine::refresh();
  }
  terminate();
  return 0;
}
