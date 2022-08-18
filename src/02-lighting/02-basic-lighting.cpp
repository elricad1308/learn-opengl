#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_s.h>
#include "../../include/fps_camera.h"

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void mouse_callback (GLFWwindow *window, double xPosIn, double yPosIn);
void process_input (GLFWwindow *window);
void scroll_callback (GLFWwindow *window, double xOffset, double yOffset);

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main ()
{
  // Variables
  float currentFrame;
  float vertices[] = {
    -0.5f, -0.5f,  0.5f,  // 0
    -0.5f,  0.5f,  0.5f,  // 1  1 2
     0.5f,  0.5f,  0.5f,  // 2  0 3
     0.5f, -0.5f,  0.5f,  // 3
    -0.5f, -0.5f, -0.5f,  // 4
    -0.5f,  0.5f, -0.5f,  // 5  5 6
     0.5f,  0.5f, -0.5f,  // 6  4 7
     0.5f, -0.5f, -0.5f   // 7
  };
  int indices[] = {
    0, 1, 3, 1, 2, 3, // Front
    4, 5, 7, 5, 6, 7, // Back
    0, 1, 4, 0, 4, 5, // Left
    2, 3, 6, 3, 6, 7, // Right
    1, 2, 6, 1, 5, 6, // Top
    0, 4, 7, 0, 3, 7  // Bottom
  };
  unsigned int EBO, VBO, VAO[2];
  GLFWwindow *window;  
  glm::mat4 lightModel, model, view, projection;
  glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basic lighting", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "GLFW no pudo crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO[0]);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Shaders
  Shader objectShader("../shaders/textureless.vs.glsl", "../shaders/textureless.fs.glsl");
  Shader lightShader("../shaders/light.vs.glsl", "../shaders/light.fs.glsl");
  
  objectShader.use();
  objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  objectShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  // Transformaciones
  lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    process_input(window);

    model = glm::mat4(1.0f);
    view = camera.GetViewMatrix();
    projection = glm::perspective(
      glm::radians(camera.Zoom),
      (float)SCR_WIDTH / (float)SCR_HEIGHT, 
      0.1f, 100.0f
    );

    objectShader.use();
    objectShader.setMat4("model", model);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);

    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    lightShader.use();
    lightShader.setMat4("model", lightModel);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);

    glBindVertexArray(VAO[1]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  objectShader.clear();
  lightShader.clear();
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(2, VAO);
  glfwTerminate();

  return 0;
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
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;

  lastX = xPos;
  lastY = yPos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

void process_input (GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(UP, deltaTime);
  }

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(DOWN, deltaTime);
  }
}

void scroll_callback (GLFWwindow *window, double xOffset, double yOffset)
{
  camera.ProcessMouseScroll(static_cast<float>(yOffset));
}
