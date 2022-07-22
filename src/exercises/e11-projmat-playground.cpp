#include <iostream>
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
  int channels, height, width;
  int indices[] = {
    // TOP
    8, 10, 11, 
    8, 9, 10, 

    // BOTTOM
    12, 14, 15,
    12, 13, 14,
    
    // FRONT
    0, 1, 3,  // BL - TL - BR
    1, 2, 3,  // TL - TR - BL

    // BACK
    4, 5, 7,  // BR - TR - BL
    5, 6, 7,  // TR - TL - BL    

    // LEFT
    0, 4, 1,  // FBL - BBR - FTL
    1, 5, 4,  // FTL - BTR - BBR

    // RIGHT
    7, 3, 6,  // BBL - FBR - BTL
    6, 2, 3,  // BTL - FTR - FBR 
  };
  float vertex_data[] = {
    // FRONT
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // 0  - BL
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // 1  - TL
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // 2  - TR
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // 3  - BR

    // BACK
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 4  - BR
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // 5  - TR
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  // 6  - TL
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 7  - BL

    // TOP
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  // 8
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // 9
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  // 10
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  // 11

    // BOTTOM
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 12
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // 13
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,   // 14
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // 15
  };
  unsigned char *imageData;
  unsigned int textures[2];
  unsigned int matLocation, EBO, VAO, VBO;
  GLFWwindow *window;
  glm::mat4 model, view, projection;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ejercicio 11", NULL, NULL);
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
  Shader ourShader("../shaders/coord-system.vs.glsl", "../shaders/texture.fs.glsl");
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);  

  // Transformaciones
  matLocation = glGetUniformLocation(ourShader.ID, "view");
  view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(view));

  matLocation = glGetUniformLocation(ourShader.ID, "projection");  
  projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.f);
  glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(projection));

  matLocation = glGetUniformLocation(ourShader.ID, "model");

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
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