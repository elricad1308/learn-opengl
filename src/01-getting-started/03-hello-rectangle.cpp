#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

/**
 * @brief Código del shader de fragmentos.
 * 
 */
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

/**
 * @brief Código del shader de vértices.
 * 
 */
const char* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

/**
 * @brief Dibuja un rectángulo
 * 
 * Este modelo utiliza en Element Buffer Object (EBO) junto con un
 * conjunto de vértices definidos en un Vertex Buffer Object (VBO) para
 * dibujar un rectángulo, especificando los datos de 4 vértices y usando
 * los índices de dichos vértices para dibujar las primitivas.
 * 
 * @return int 
 */
int main ()
{
  // -------------------------------------------------------------------
  // Definición de variables
  // -------------------------------------------------------------------
  unsigned int EBO;               // Element Buffer Object
  unsigned int fragmentShader;    // Shader de fragmentos
  char infoLog[512];              // Buffer para mensajes de log
  unsigned int shaderProgram;     // Resultado de la unión de shaders
  int success;                    // Usado para revisar estados
  unsigned int VAO;               // Vertex Array Object
  unsigned int VBO;               // Vertex Buffer Object
  unsigned int vertexShader;      // Shader de vertices
  

  // Coordenadas de los vértices
  float vertices[] = {
     0.5f,  0.5f, 0.0f,     // Superior derecho
     0.5f, -0.5f, 0.0f,     // Inferior derecho
    -0.5f, -0.5f, 0.0f,     // Inferior izquierdo
    -0.5f,  0.5f, 0.0f      // Superior izquierdo
  };

  // Índices de los vértices 
  int indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  // -------------------------------------------------------------------
  // Fin definición de variables
  // -------------------------------------------------------------------


  // -------------------------------------------------------------------
  // Inicialización de glfw
  // -------------------------------------------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Rectangle", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Error al crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al inicializar GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // -------------------------------------------------------------------
  // Fin de inicialización
  // -------------------------------------------------------------------


  // -------------------------------------------------------------------
  // Creación de shaders
  // -------------------------------------------------------------------
  // Shader de vértices
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Shader de fragmentos
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Programa shader
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
  }
  glUseProgram(shaderProgram);

  // Libera recursos de los shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // -------------------------------------------------------------------
  // Fin de la creación de shaders
  // -------------------------------------------------------------------


  // -------------------------------------------------------------------
  // Creación de buffers
  // -------------------------------------------------------------------
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Establece el VAO actual
  glBindVertexArray(VAO);

  // Transfiere los vértices al GPU
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Transfiere los índices de las coordenadas al GPU
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Indica a OpenGL cómo usar los vértices
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Desvincula los buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // -------------------------------------------------------------------
  // Fin de la creación de buffers
  // -------------------------------------------------------------------


  // -------------------------------------------------------------------
  // Ciclo de rendereizado
  // -------------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibuja el rectángulo
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // -------------------------------------------------------------------
  // Fin del ciclo de renderizado
  // -------------------------------------------------------------------


  // -------------------------------------------------------------------
  // Limpieza
  // -------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  // -------------------------------------------------------------------
  // Fin de limpieza
  // -------------------------------------------------------------------

  return 0;
}

/**
 * @brief ajusta el área de renderizado al redimensionar la ventana.
 * 
 * @param window el objeto con el contexto de la ventana.
 * 
 * @param width el nuevo ancho de window
 * 
 * @param height el nuevo alto de window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

/**
 * @brief Procesa las entradas del usuario.
 * 
 * @param window el objeto con el contexto de la ventana.
 */
void process_input (GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}