#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

void inputInit();
void updateInputState(GLFWwindow* window);

bool getKeyPressed(int key);
bool getKeyReleased(int key); 
bool getKeyDown(int key);    

bool getMousePressed(int button);     
bool getMouseReleased(int button);   
bool getMouseDown(int button);      

double getMouseX(GLFWwindow*);
double getMouseY(GLFWwindow*);
#endif
