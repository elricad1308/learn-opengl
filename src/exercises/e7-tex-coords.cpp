#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/shader_s.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void process_input (GLFWwindow *window);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main ()
{
  // Variables
  int channels, height, width;
  int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  float vertex_data[] = {
    // Posiciones        // Colores         // Tex Coords
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // Inferior izq
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // Superior izq
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // Superior der
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f   // Inferior der
  };
  unsigned char *data;
  unsigned int textura1, textura2, EBO, VAO, VBO;
  GLFWwindow *window;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ejercicio 6", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Error al crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al cargar GLAD" << std::endl;
    return -1; 
  }
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // Textura 1
  glGenTextures(1, &textura1);
  glBindTexture(GL_TEXTURE_2D, textura1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("../../textures/awesomeface.png", &width, &height, &channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Error al cargar la textura" << std::endl;
  }
  stbi_image_free(data);
  
  // Textura 2
  glGenTextures(1, &textura2);
  glBindTexture(GL_TEXTURE_2D, textura2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("../../textures/container.jpg", &width, &height, &channels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Error al cargar la textura 2" << std::endl;
  }
  stbi_image_free(data);

  // Shaders
  Shader ourShader("../shaders/texture.vs.glsl", "../shaders/texture.fs.glsl");
  ourShader.use();

  // Vincula los uniforms del shader program con las texturas
  ourShader.setInt("texture1", 1);
  ourShader.setInt("texture2", 0);

  // Buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VAO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activa los canales de textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textura2);

    glBindVertexArray(VAO);    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  ourShader.clear();
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteTextures(1, &textura1);
  glDeleteTextures(1, &textura2);
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