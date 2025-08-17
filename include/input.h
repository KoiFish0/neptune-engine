#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#include <globals.h>

bool currentState[GLFW_KEY_LAST + 1] = { false };
bool previousState[GLFW_KEY_LAST + 1] = { false };

bool currentMouse[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
bool previousMouse[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

float delta;
float lastTime = glfwGetTime();

class Input {
private:
  /* Delta time is the time between frames */
  static float getDelta() {
    float delta = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    return delta;
  }

public:
  /* Zero out all the values for input tracking */
  static void inputInit() {
    for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
      currentState[i] = false;
      previousState[i] = false;
    }
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i) {
      currentMouse[i] = false;
      previousMouse[i] = false;
    }
  }

  /* Retrieve the current input state using GLFW for all keys up to GLFW_KEY_LAST which equates to key 348, the last in the sequence */
  static void updateInputState(GLFWwindow* window) {
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

  static bool getKeyPressed(int key) {
    return currentState[key] && !previousState[key];
  }

  static bool getKeyReleased(int key) {
    return !currentState[key] && previousState[key];
  }

  static bool getKeyDown(int key) {
    return currentState[key];
  }

  static bool getMousePressed(int button) {
    return currentMouse[button] && !previousMouse[button];
  }

  static bool getMouseReleased(int button) {
    return !currentMouse[button] && previousMouse[button];
  }

  static bool getMouseDown(int button) {
    return currentMouse[button];
  }

  static double getMouseX() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return xpos;
  }

  static double getMouseY() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return ypos;
  }
};

#endif
