#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 _position)
    : position(_position),
      up(0, 1, 0),
      front(0, 0, -1),
      right(1, 0, 0),
      rotation(glm::identity<glm::quat>()),
      projectionMatrix(1),
      viewMatrix(1) {}

void Camera::initialize(float aspectRatio) {
  updateProjectionMatrix(aspectRatio);
  updateViewMatrix();
}

void Camera::move(GLFWwindow* window) {
  bool ismoved = false;
  // Mouse part
  static double lastx = 0, lasty = 0;
  if (lastx == 0 && lasty == 0) {
    glfwGetCursorPos(window, &lastx, &lasty);
  } else {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float dx = mouseMoveSpeed * static_cast<float>(xpos - lastx);
    float dy = mouseMoveSpeed * static_cast<float>(lasty - ypos);
    lastx = xpos;
    lasty = ypos;
    if (dx != 0 || dy != 0) {
      ismoved = true;
      glm::quat rx(glm::angleAxis(dx, glm::vec3(0, -1, 0)));
      glm::quat ry(glm::angleAxis(dy, glm::vec3(1, 0, 0)));
      rotation = rx * rotation * ry;
    }
  }
  // Keyboard part
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += front * keyboardMoveSpeed;
    ismoved = true;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= front * keyboardMoveSpeed;
    ismoved = true;
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= right * keyboardMoveSpeed;
    ismoved = true;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += right * keyboardMoveSpeed;
    ismoved = true;
  }
  // Update view matrix if moved
  if (ismoved) {
    updateViewMatrix();
  }
  
}

void Camera::updateViewMatrix() {
  constexpr glm::vec3 original_front(0, 0, -1.);
  constexpr glm::vec3 original_up(0, 1, 0);
  /* TODO#1-1: Calculate lookAt matrix
   *       1. Rotate original_front and original_up using this->rotation.
   *       2. Calculate right vector by cross product.
   *       3. Calculate view matrix with posision.
   * Hint:
   *       You can calculate the matrix by hand, or use
   *       glm::lookAt (https://glm.g-truc.net/0.9.9/api/a00247.html#gaa64aa951a0e99136bba9008d2b59c78e)
   * Note: You must not use gluLookAt
   */
  glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
  glm::vec3 newFront = glm::mat3(rotationMatrix) * original_front;
  glm::vec3 newUp = glm::mat3(rotationMatrix) * original_up;
  // 2. Calculate the right vector by cross product.
  right = glm::cross(newFront, newUp);
  front = newFront;
  up = newUp;
  // 3. Calculate the view matrix with position.
  viewMatrix = glm::mat4(
      right.x, up.x, -front.x, 0.0f, 
      right.y, up.y, -front.y, 0.0f, 
      right.z, up.z, -front.z, 0.0f,
      -glm::dot(right, position), -glm::dot(up, position), glm::dot(front, position), 1.0f);
}

 




void Camera::updateProjectionMatrix(float aspectRatio) {
  constexpr float FOV = glm::radians(45.0f);
  constexpr float zNear = 0.1f;
  constexpr float zFar = 100.0f;
  /* TODO#1-2: Calculate perspective projection matrix
   * Hint: You can calculate the matrix by hand, or use
   *       glm::perspective (https://glm.g-truc.net/0.9.9/api/a00243.html#ga747c8cf99458663dd7ad1bb3a2f07787)
   * Note: You must not use gluPerspective
   */
// Calculate the components of the projection matrix.
  float tanHalfFOV = tan(FOV / 2.0f);
  float rangeInv = 1.0f / (zNear - zFar);
  // Construct the elements of the projection matrix.
  float a = 1.0f / (tanHalfFOV * aspectRatio);
  float b = 1.0f / tanHalfFOV;
  float c = (zNear + zFar) * rangeInv;
  float d = (2.0f * zNear * zFar) * rangeInv;
  // Fill the projection matrix.
  projectionMatrix = glm::mat4(a, 0.0f, 0.0f, 0.0f, 0.0f, b, 0.0f, 0.0f, 0.0f, 0.0f, c, -1.0f, 0.0f, 0.0f, d, 0.0f);
}