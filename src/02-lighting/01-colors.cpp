#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_s.h>
#include "../../include/camera.h"

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void mouse_callback (GLFWwindow *window, double xPos, double yPos);
void process_input (GLFWwindow *window);
void scroll_callback (GLFWwindow *window, double xOffset, double yOffset);

const int SCR_HEIGHT = 720;
const int SCR_WIDTH = 1280;

bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main ()
{
  // Variables
  int indices[] = {
    0, 1, 3,
    1, 2, 3,

    4, 5, 7,
    5, 6, 7,

    0, 1, 5,
    0, 4, 5,

    2, 3, 6,
    3, 6, 7,

    1, 5, 6,
    1, 2, 6,

    0, 4, 7,
    0, 3, 7
  };
  float currentFrame;
  float vertex_data[] = {
    -0.5f, -0.5f,  0.5f,  // 0
    -0.5f,  0.5f,  0.5f,  // 1  1 2
     0.5f,  0.5f,  0.5f,  // 2  0 3
     0.5f, -0.5f,  0.5f,  // 3

    -0.5f, -0.5f, -0.5f,  // 4
    -0.5f,  0.5f, -0.5f,  // 5  5 6
     0.5f,  0.5f, -0.5f,  // 6  4 7
     0.5f, -0.5f, -0.5f   // 7
  };
  unsigned int EBO, VAO[2], VBO;
  GLFWwindow *window;
  glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
  glm::mat4 model, view, projection, lightModel;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Error al crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al cargar las funciones de OpenGL" << std::endl;
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
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
  Shader ourShader("../shaders/textureless.vs.glsl", "../shaders/textureless.fs.glsl");
  ourShader.use();

  ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  Shader lightShader("../shaders/textureless.vs.glsl", "../shaders/light.fs.glsl");


  lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    process_input(window);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();
    model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));

    view = camera.GetViewMatrix();

    projection = glm::perspective(
      glm::radians(camera.Zoom), 
      (float)SCR_WIDTH / (float)SCR_HEIGHT,
      0.1f, 100.0f
    );

    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

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
  lightShader.clear();
  ourShader.clear();
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(2, VAO);
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback (GLFWwindow *window, double xPos, double yPos)
{
  float x = static_cast<float>(xPos);
  float y = static_cast<float>(yPos);

  if (firstMouse)
  {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  float xOffset = x - lastX;
  float yOffset = lastY - y;

  lastX = x;
  lastY = y;

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

  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

void scroll_callback (GLFWwindow *window, double xOffset, double yOffset)
{
  camera.ProcessMouseScroll(static_cast<float>(yOffset));
}