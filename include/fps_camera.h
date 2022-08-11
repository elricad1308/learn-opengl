#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// Default camera values
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float SPEED = 2.5f;
const float YAW = -90.0f;
const float ZOOM = 45.0f;

class Camera
{
public:
  // Camera attributes
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
    updateCameraVectors();
  }

  glm::mat4 GetViewMatrix ()
  {
    return glm::lookAt(Position, Position + Front, Up);
    /*glm::mat4 rotation(1.0f);
    glm::mat4 translation(1.0f);

    rotation[0] = glm::vec4(Right, 0.0f);
    rotation[1] = glm::vec4(Up, 0.0f);
    rotation[2] = glm::vec4(Position + Front, 0.0f);

    translation = glm::translate(translation, -Position);

    return rotation * translation;*/
  }

  void ProcessKeyboard (Camera_Movement direction, float deltaTime)
  {
    float velocity = deltaTime * MovementSpeed;

    if (direction == FORWARD)
    {
      Position += Front * velocity;
    }

    if (direction == BACKWARD)
    {
      Position -= Front * velocity;
    }

    if (direction == LEFT)
    {
      Position -= Right * velocity;
    }

    if (direction == RIGHT)
    {
      Position += Right * velocity;
    }

    Position.y = 0.0f;
  }

  void ProcessMouseMovement (float xOffset, float yOffset, GLboolean constrainPitch = true)
  {
    Yaw += xOffset * MouseSensitivity;
    Pitch += yOffset * MouseSensitivity;

    if (constrainPitch && Pitch > 89.0f)
    {
      Pitch = 89.0f;
    }
    if (constrainPitch && Pitch < -89.0f)
    {
      Pitch = -89.0f;
    }

    updateCameraVectors();
  }

  void ProcessMouseScroll (float yOffset)
  {
    Zoom -= (float)yOffset;

    if (Zoom < 1.0f)
    {
      Zoom = 1.0f;
    }
    if (Zoom > 45.0f)
    {
      Zoom = 45.0f;
    }
  }

private:
  void updateCameraVectors()
  {
    glm::vec3 front;

    // Calculate the new Front vector
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);

    // Also recalculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }

};

#endif