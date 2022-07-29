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
    updateCameraVectors();
  }

  /**
   * @brief Get the View Matrix object
   * 
   * @return glm::mat4 
   */
  glm::mat4 GetViewMatrix ()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  /**
   * @brief Process input received from any keyboard-like input system.
   * 
   * Accepts input parameter in the form of camera defined ENUM (to abs-
   * tract it from windowing systems).
   * 
   * @param direction the input received
   * 
   * @param deltaTime time elapsed since the previous input signal
   */
  void ProcessKeyboard (Camera_Movement direction, float deltaTime)
  {
    float velocity = MovementSpeed * deltaTime;

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
  }

  /**
   * @brief Process input received from a mouse input system.
   * 
   * Expects the offset value in both the X and Y directions.
   * 
   * @param xOffset change on the X axis since the last movement.
   * 
   * @param yOffset chance on the Y axis since the last movement.
   * 
   * @param constrainPitch whether or not the pitch shoudl be cons-
   *   trained to avoid reaching 90 degrees both up and down. It helps
   *   prevent gimball locks. Defaults to true
   */
  void ProcessMovement (float xOffset, float yOffset, GLboolean constrainPitch = true)
  {
    Yaw += xOffset * MouseSensitivity;
    Pitch += yOffset * MouseSensitivity;

    // If told so, constrain the pitch to avoid screen flipping
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

  /**
   * @brief Process input received from a mouse scroll-wheel event.
   * 
   * Only requires input on the vertical wheel axis.
   * 
   * @param yOffset 
   */
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
/**
 * @brief Calculates the front vector from the camera's Euler Angles
 * 
 */
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