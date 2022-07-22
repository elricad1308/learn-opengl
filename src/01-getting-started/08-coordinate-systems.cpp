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
  // -------------------------------------------------------------------
  int channels, height, width;
  int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  float vertex_data[] = {
    // Posicion          // Color           // Tex Coords
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f
  };
  unsigned char *imageData;
  unsigned int locMat, textures[2], EBO, VAO, VBO;
  GLFWwindow *window;

  // Inicialización
  // -------------------------------------------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sistemas coordenados", NULL, NULL);
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
  // glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // glEnableVertexAttribArray(2);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

  glBindTexture(GL_TEXTURE_2D, 0);

  // Shaders
  Shader ourShader("../shaders/coord-system.vs.glsl", "../shaders/texture.fs.glsl");
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  // Transformaciones
  // Matriz de modelo: mueve el plano de sus coordenadas locales al
  // sistema coordenado del mundo
  glm::mat4 model = glm::mat4(1.0f);
  // Rota al plano 55 grados en el eje X
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));  

  // Matriz de vista: posiciona la escena desde el punto de vista del
  // observador (esto es lo que comúnmente llamamos 'cámara')
  glm::mat4 view = glm::mat4(1.0f);

  // Mueve la escena entera hacia el frente: esto es equivalente a 
  // moverse hacia atrás con la cámara. OpenGL es un sistema de mano
  // derecha, por lo que el eje positivo X está a la derecha, el eje
  // positivo Y está hacia arriba, y el eje positivo Z está hacia el
  // frente (saliendo de la pantalla)
  // Mover la escena 3 unidades en el eje Z negativo entonces equivale
  // a posicionar la cámara hacia atrás (pues estamos alejando la
  // escena hacia "dentro" de la pantalla)
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  // Matriz de proyección: aplica la perspectiva a la escena
  glm::mat4 projection;
  projection = glm::perspective(
    // Ángulo del punto de vista (POV). 45 grados es un valor común que
    // da una perspectiva natural.
    glm::radians(45.0f), 
    
    // Aspect radio: para obtenerlo simplemente dividimos el ancho de
    // la pantalla entre el alto
    (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 
    
    // Distancia del plano 'Near' o cercano: este valor indica la
    // posición inicial a partir del cual se calculará el valor de
    // proyección, es decir, que tan cerca debe estar un elemento a la
    // 'cámara' para aparecer en la escena.
    0.1f, 
    
    // Distancia del plano 'far' o lejano: este valor indica la posición
    // final hasta la cual se calculará el valor de proyección, es decir,
    // el horizonte. Cualquier elemento posicionado más allá de este
    // valor no aparecerá en la escena.
    100.0f
  );

  locMat = glGetUniformLocation(ourShader.ID, "view");
  glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(view));
  locMat = glGetUniformLocation(ourShader.ID, "projection");
  glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(projection));
  locMat = glGetUniformLocation(ourShader.ID, "model");

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

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
      // Crea transformaciones
      model = glm::mat4(1.0f);    
      // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
      model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
      glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(model));    

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  // -------------------------------------------------------------------
  ourShader.clear();
  glDeleteTextures(2, textures);
  glDeleteVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);
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