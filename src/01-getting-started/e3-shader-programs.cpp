#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void process_input (GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
  "layout (position = 0) in vec3 aPos;\n"
  "void main ()\n"
  "{\n"
  "  gl_Position = vec4(aPos.xyz, 1.0);\n"
  "}\0";

const char *orangeFragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main ()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

const char *yellowFragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main ()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.8f, 0.0f, 1.0f);\n"
  "}\0";

int main ()
{
  // Variables
  char infoLog[512];
  int success;
  unsigned int vertexShader, orangeFragShader, yellowFragShader;
  unsigned int orangeShaderProgram, yellowShaderProgram;
  GLFWwindow *window;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(800, 600, "Ejercicio 3", NULL, NULL);

  if (window == NULL)
  {
    std::cout << "Error al crear la ventana de la aplicación" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al cargar la librería GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Shaders
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  orangeFragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(orangeFragShader, 1, &orangeFragmentShaderSource, NULL);
  glCompileShader(orangeFragShader);
  glGetShaderiv(orangeFragShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(orangeFragShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::ORANGE_FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  yellowFragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(yellowFragShader, 1, &yellowFragmentShaderSource, NULL);
  glCompileShader(yellowFragShader);
  glGetShaderiv(yellowFragShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(yellowFragShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::YELLOW_FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  orangeShaderProgram = glCreateProgram();
  glAttachShader(orangeShaderProgram, vertexShader);
  glAttachShader(orangeShaderProgram, orangeFragShader);
  glLinkProgram(orangeShaderProgram);
  glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
  
  if (!success)
  {
    glGetProgramInfoLog(orangeShaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::ORANGE_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  yellowShaderProgram = glCreateProgram();
  glAttachShader(yellowShaderProgram, vertexShader);
  glAttachShader(yellowShaderProgram, yellowFragShader);
  glLinkProgram(yellowShaderProgram);
  glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::YELLOW_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Limpieza
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}