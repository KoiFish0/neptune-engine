#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"

struct Camera {
  glm::vec3 position;
  glm::vec3 lookVector; // Look direction 
  glm::vec3 up; // Up vector relative to the look direction; used for pitch/roll
  
  // Camera properties that affect perspective projection 
  float fov;
  float aspectRatio;
  float nearPlane;
  float farPlane;

  // Create a camera and initialize it with the provide values
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
};

#endif // CAMERA_H
