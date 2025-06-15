#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"

struct Camera {
  glm::vec3 position;
  glm::vec3 lookVector; // Look direction 
  glm::vec3 up; // Up vector relative to the look direction; used for pitch/roll

  float fov;
  float aspectRatio;
  float nearPlane;
  float farPlane;

  // Methods
  Camera create(glm::vec3 position, glm::vec3 lookVector, glm::vec3 up, float fov, float aspectRatio, float nearPlane, float farPlane) {
    Camera camera;

    camera.position = position;
    camera.lookVector = lookVector;
    camera.up = up;
    camera.fov = fov;
    camera.aspectRatio = aspectRatio;
    camera.nearPlane = nearPlane;
    camera.farPlane = farPlane;

    return camera;
  }

  glm::mat4 getViewMatrix() {
    return glm::lookAt(position, position + glm::normalize(lookVector), up);
  } 
  
  glm::mat4 getProjectionMatrix() {
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  }
  
  // TODO Create some methods to transform the camera (Don't forget delta; Implement in globals.h)
};

#endif // CAMERA_H
