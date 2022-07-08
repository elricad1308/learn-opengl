#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void process_input (GLFWwindow *window);

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec4 vertexColor;\n"
  "void main ()\n"
  "{\n"
  "  FragColor = vertexColor;\n"
  "}\0";

const char *uniformFragShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "uniform vec4 ourColor;\n"
  "void main ()\n"
  "{\n"
  "  FragColor = ourColor;\n"
  "}\0";

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "out vec4 vertexColor;\n"
  "void main ()\n"
  "{\n"
  "  gl_Position = vec4(aPos, 1.0);\n"
  "  vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
  "}\0";

int main ()
{
  // Variables
  char infoLog[512];
  float greenValue, timeValue;
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };
  int success, vertexColorLocation;
  unsigned int vertexShader, fragmentShader, programShader;
  unsigned int VAO, VBO;
  GLFWwindow *window;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(800, 600, "Shaders", NULL, NULL);

  if (window == NULL)
  {
    std::cout << "Error al crear el contexto" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al inicializar GLAD" << std::endl;
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
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << std::endl;
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &uniformFragShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << std::endl;
  }

  programShader = glCreateProgram();
  glAttachShader(programShader, vertexShader);
  glAttachShader(programShader, fragmentShader);
  glLinkProgram(programShader);
  glGetProgramiv(programShader, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(programShader, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glUseProgram(programShader);
  vertexColorLocation = glGetUniformLocation(programShader, "ourColor");

  // Buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    timeValue = glfwGetTime();
    greenValue = (sin(timeValue) / 2.0f) + 0.5f;    
    glUniform4f(vertexColorLocation, 0.3f, greenValue, 0.3f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents(); 
  }

  // Opcional, muestra cantidad máxima de atributos de vértices
  // soportados por el hardware.
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &success);
  std::cout << "Maximum number of vertex attributes supported: " << success << std::endl;

  // Limpieza
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(programShader);
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void process_input (GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}