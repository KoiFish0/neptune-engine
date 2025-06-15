#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.h"
#include "globals.h"

// Some variables for tracking current and previous states for buttons/keys
static bool currentState[GLFW_KEY_LAST + 1] = { false };
static bool previousState[GLFW_KEY_LAST + 1] = { false };

static bool currentMouse[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
static bool previousMouse[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
// Delta time
float lastTime = glfwGetTime();

float getDelta() {
  float delta = glfwGetTime() - lastTime;
  lastTime = glfwGetTime();
  
  return delta;
}

float delta;

// Zero out all the values for input tracking
void inputInit() {
    for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
        currentState[i] = false;
        previousState[i] = false;
    }
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i) {
        currentMouse[i] = false;
        previousMouse[i] = false;
    }
}
// Retrieve the current input state using GLFW for all keys up to GLFW_KEY_LAST which equates to key 348, the last in the sequence
void updateInputState(GLFWwindow* window) {
    glfwPollEvents();
    delta = getDelta();

    for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
        previousState[i] = currentState[i];
        currentState[i] = (glfwGetKey(window, i) == GLFW_PRESS);
    }
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i) {
        previousMouse[i] = currentMouse[i];
        currentMouse[i] = (glfwGetMouseButton(window, i) == GLFW_PRESS);
    }
}
// Check for key/mouse presses, releases, and whether it's held down
bool getKeyPressed(int key) {
    return currentState[key] && !previousState[key];
}

bool getKeyReleased(int key) {
    return !currentState[key] && previousState[key];
}

bool getKeyDown(int key) {
    return currentState[key];
}

bool getMousePressed(int button) {
    return currentMouse[button] && !previousMouse[button];
}

bool getMouseReleased(int button) {
    return !currentMouse[button] && previousMouse[button];
}

bool getMouseDown(int button) {
    return currentMouse[button];
}

// Mouse positions
double getMouseX(GLFWwindow* window) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return xpos;
}

double getMouseY(GLFWwindow* window) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return ypos;
}

