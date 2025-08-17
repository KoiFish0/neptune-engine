#ifndef GLOBALS_H
#define GLOBALS_H

#include <GLFW/glfw3.h>

#include <camera.h>

extern int width, height;

extern float delta;

// A pointer to the window; gives a handle to the window to other files
extern GLFWwindow* window;

// Current active camera; allows multiple cameras and swapping between them
extern Camera activeCamera;

// Whether to print debug info intended for development of the engine
bool debugPrint;

#endif
