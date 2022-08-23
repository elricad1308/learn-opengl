#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../include/camera.h"

void click_callback (GLFWwindow *window, int button, int action, int mods);
void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void mouse_callback (GLFWwindow *window, double xPosIn, double yPosIn);
void process_input (GLFWwindow *window);
void scroll_callback (GLFWwindow *window, double xOffset, double yOffset);

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

bool firstMouse = true;
float deltaTime;
float lastFrame = 0.0f;
float lastX = static_cast<float>(SCR_WIDTH / 2);
float lastY = static_cast<float>(SCR_HEIGHT / 2);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main ()
{
  // Variables
  float currentFrame;
  unsigned int VAO[2], VBO;
  GLFWwindow *window;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ejercicio 13", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "GLFW no pudo crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetMouseButtonCallback(window, click_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "GLAD no pudo cargar las funciones de OpenGL" << std::endl;
    glfwTerminate();
    return -1;
  }
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glEnable(GL_DEPTH_TEST);

  // Buffers
  glGenVertexArrays(2, VAO);
  glGenBuffers(1, &VBO);

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    process_input(window);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(2, VAO);
  glfwTerminate();

  return 0;
}

void click_callback (GLFWwindow *window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
}

void framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback (GLFWwindow *window, double xPosIn, double yPosIn)
{
  float xPos = static_cast<float>(xPosIn);
  float yPos = static_cast<float>(yPosIn);

  if (firstMouse)
  {
    firstMouse = false;
    lastX = xPos;
    lastY = yPos;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

void process_input (GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void scroll_callback (GLFWwindow *window, double xOffset, double yOffset)
{
  camera.ProcessMouseScroll(static_cast<float>(yOffset));
}