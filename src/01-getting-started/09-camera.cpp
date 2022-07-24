#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_s.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void process_input (GLFWwindow *window);

int main ()
{
  // Variables
  // -------------------------------------------------------------------
  int channels, height, width;
  int indices[] = {
    // Frente
    0, 1, 2,
    0, 2, 3,
    // Atrás
    4, 5, 6,
    4, 6, 7,
    // Izquierda
    0, 6, 7,
    0, 1, 6,
    // Derecha
    3, 4, 5,
    2, 3, 5,
    // Arriba/Abajo
    8, 9, 10,
    8, 10, 11,
  };
  float camX, camZ;
  float vertex_data[] = {
    // Frente
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // 0
    -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // 1    1 2
     0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // 2    0 3
     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // 3

     // Atrás
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 4
    0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 5    6 5
   -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 6    7 4
   -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 7

   // Arriba
   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // 8
   -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 9    9 10
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 10   8 11
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 11
  };
  unsigned char *imageData;
  unsigned int matLocation, textures[2], EBO, VAO, VBO;
  GLFWwindow *window;
  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,   0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f,  -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f,  -3.5f),
    glm::vec3(-1.7f,  3.0f,  -7.5f),
    glm::vec3( 1.3f, -2.0f,  -2.5f),
    glm::vec3( 1.5f,  2.0f,  -2.5f),
    glm::vec3( 1.5f,  0.2f,  -1.5f),
    glm::vec3(-1.3f,  1.0f,  -1.5f),
  };
  glm::mat4 model, view, projection;

  // Inicialización
  // -------------------------------------------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Camera", NULL, NULL);
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
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glEnable(GL_DEPTH_TEST);

  // Buffers
  // -------------------------------------------------------------------
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Texturas
  // -------------------------------------------------------------------
  glGenTextures(2, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  imageData = stbi_load("../../textures/container.jpg", &width, &height, &channels, 0);
  if (imageData)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Error al cargar la textura 1" << std::endl;
  }
  stbi_image_free(imageData);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  imageData = stbi_load("../../textures/awesomeface.png", &width, &height, &channels, 0);

  if (imageData)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Error al cargar la textura 2" << std::endl;
  }
  stbi_image_free(imageData);

  // Shaders
  // -------------------------------------------------------------------
  Shader ourShader("../shaders/cube.vs.glsl", "../shaders/texture.fs.glsl");
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  // Transformaciones
  // -------------------------------------------------------------------
  projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.f);
  matLocation = glGetUniformLocation(ourShader.ID, "projection");
  glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(projection));

  // Ciclo de renderizado
  // -------------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    // Cámara
    matLocation = glGetUniformLocation(ourShader.ID, "view");
    camX = sin(glfwGetTime()) * 10.0f;
    camZ = cos(glfwGetTime()) * 10.0f;

    // Genera una matriz 'lookAt', que nos permite transformar las coor-
    // denadas del sistema global a las coordenadas del punto de vista
    // de la cámara, lo que permite posicionar la escena de modo tal que
    // simule ser visto a través de una cámara
    view = glm::lookAt(
      // Vector de posición de la cámara, indica las coordenadas dentro
      // del mundo en el cual se encuentra colocada la cámara
      glm::vec3(camX, 0.0f, camZ),

      // Vector de dirección de la cámara, indica las coordenadas dentro
      // del mundo hacia las cuales la cámara está mirando
      glm::vec3(0.0f, 0.0f, 0.0f),

      // Vector 'up', indica la posición que se considera 'arriba' den-
      // tro del sistema coordenado del mundo, es utilizado por la li-
      // brería GLM para calcular vectores ortogonales hacia arriba y la
      // derecha con origen en la cámara, los cuales constituyen su eje
      // coordenado
      glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(view));

    matLocation = glGetUniformLocation(ourShader.ID, "model");
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      if (i == 0)
      {
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
      }
      else
      {
        model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
      }      
      glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(model));

      ourShader.setFloat("invert", 1.0f);
      glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
      // Dibuja el fondo
      ourShader.setFloat("invert", -1.0f);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(int)));      
    }
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  // -------------------------------------------------------------------
  ourShader.clear();
  glDeleteTextures(2, textures);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
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