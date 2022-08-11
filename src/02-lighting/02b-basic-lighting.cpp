#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <shader_s.h>
#include <camera.h>

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void mouse_callback (GLFWwindow *window, double xPosIn, double yPosIn);
void process_input (GLFWwindow *window);
void scroll_callback (GLFWwindow *window, double xOffset, double yOffset);

bool firstMouse;
float deltaTime, lastFrame, lastX, lastY;
Camera camera(glm::vec3(0.0f, 0.0f, 0.3f));

int main ()
{
  GLFWwindow *window;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basic Lighting", NULL, NULL);  
  if (window == NULL)
  {
    std::cout << "Error al crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al cargar las funciones de OpenGL" << std::endl;
    glfwTerminate();
    return -1;
  }
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    process_input(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
}

void framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback (GLFWwindow *window, double xPosIn, double yPosIn)
{
  
}

void process_input (GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}