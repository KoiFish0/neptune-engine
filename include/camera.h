#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
  glm::vec3 position;
  glm::vec3 lookVector;
  glm::vec3 up; // Up vector relative to the look direction; used for pitch/roll
  
  float fov;
  float aspectRatio;
  float nearPlane;
  float farPlane;

  Camera(glm::vec3 position, glm::vec3 lookVector, glm::vec3 up, float fov, float aspectRatio, float nearPlane, float farPlane)
    : position(position), 
    lookVector(lookVector), 
    up(up), 
    fov(fov),
    aspectRatio(aspectRatio),
    nearPlane(nearPlane),
    farPlane(farPlane) {
  }

  glm::mat4 getViewMatrix() {
    return glm::lookAt(position, position + glm::normalize(lookVector), up);
  } 
  
  glm::mat4 getProjectionMatrix() {
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  }
};

#endif
