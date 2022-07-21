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

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void process_input (GLFWwindow *window);

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

int main ()
{
  // Variables
  // -------------------------------------------------------------------
  int channels, height, width;
  int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  float scale, time;
  float vertex_data[] = {
    // Posicion          // Color           // Tex Coord
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Inf izq
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Sup Izq
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Sup der
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f  // Inf der
  };
  unsigned char *imageData;
  unsigned int texturas[2], transformLoc, EBO, VAO, VBO;
  glm::mat4 trans;
  GLFWwindow *window;

  // Inicialización
  // -------------------------------------------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ejercicio 10", NULL, NULL);
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
    return -1;
  }

  // Buffers
  // -------------------------------------------------------------------
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Texturas
  // -------------------------------------------------------------------
  glGenTextures(2, texturas);
  glBindTexture(GL_TEXTURE_2D, texturas[0]);
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

  glBindTexture(GL_TEXTURE_2D, texturas[1]);
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
  Shader ourShader("../shaders/transform.vs.glsl", "../shaders/texture.fs.glsl");
  ourShader.use();

  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texturas[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texturas[1]);
  transformLoc = glGetUniformLocation(ourShader.ID, "transform");

  // Ciclo de renderizado
  // -------------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    time = (float)glfwGetTime();

    trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    scale = static_cast<float>(sin(time));
    trans = glm::mat4(1.0f);    
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  // -------------------------------------------------------------------
  ourShader.clear();
  glDeleteTextures(2, texturas);
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