#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Define several possible options for camera movement. Used as abstrac-
// tion to stay away from window-system specific input methods.
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float SPEED = 2.5f;
const float YAW = -90.0f;
const float ZOOM = 45.0f;

/**
 * @brief Implements a camera
 * 
 * An abstract camera class that processes input and calculates the co-
 * rresponding Euler Angles, Vector and Matrices for use in OpenGL.
 * 
 */
class Camera
{
public:
  // Camera attributs
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // Euler angles
  float Pitch;
  float Yaw;

  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Constructor with vectors
  Camera
  (
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW,
    float pitch = PITCH
  ) : 
  Front(glm::vec3(0.0f, 0.0f, -1.0f)),
  MovementSpeed(SPEED),
  MouseSensitivity(SENSITIVITY),
  Zoom(ZOOM)
  {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors(); 
  }

  // Constructor with scalar values
  Camera
  (
    float posX, float posY, float posZ, 
    float upX, float upY, float upZ, 
    float yaw, float pitch
  ) :
  Front(glm::vec3(0.0f, 0.0f, -1.0f)),
  MovementSpeed(SPEED),
  MouseSensitivity(SENSITIVITY),
  Zoom(ZOOM)
  {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVector();
  }

private:
  void updateCameraVectors()
  {

  }
};

#endif