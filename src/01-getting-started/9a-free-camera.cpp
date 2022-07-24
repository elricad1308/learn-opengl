#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_s.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void drawCube (unsigned int shaderId);
void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void mouse_callback (GLFWwindow *window, double xPos, double yPos);
void process_input (GLFWwindow *window, glm::vec3 *cameraPos, glm::vec3 *cameraFront, glm::vec3 *cameraUp);

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const float SENSITIVITY = 0.01f;

bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_WIDTH / 2;
float pitch = 0.0f;
float yaw = 0.0f;

int main ()
{
  // Variables
  // -------------------------------------------------------------------
  float currentFrame;
  float vertex_data[] = {
    // Right
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // 0  
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // 1    1 2
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // 2    0 3
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // 3
    // Top
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   // 4
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // 5    5 6
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // 6    4 7
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // 7
    // Front
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // 8
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // 9    9 10
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // 10   8 11
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // 11
  };
  int channels, height, width;
  int indices[] = {
    // Right
    0, 1, 3,
    1, 2, 3,
    // Top
    4, 5, 7,
    5, 6, 7,
    // Front
    8, 9, 11,
    9, 10, 11
  };
  unsigned char *imageData;
  unsigned int matLocation, textures[2], EBO, VAO, VBO;
  GLFWwindow *window;
  glm::mat4 model, view, projection;
  glm::vec3 cameraPos, cameraFront, cameraUp, direction;

  // Inicialización
  // -------------------------------------------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Free camera", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Error al crear la ventana"  << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al cargar las funciones de OpenGL" << std::endl;
    glfwTerminate();
    return -1;
  }
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
  stbi_image_free(imageData);

  // Shaders
  // -------------------------------------------------------------------
  Shader ourShader("../shaders/cube.vs.glsl", "../shaders/texture.fs.glsl");
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);

  // Transformaciones
  // -------------------------------------------------------------------
  projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
  matLocation = glGetUniformLocation(ourShader.ID, "projection");
  glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(projection));

  // Vectores para el posicionamiento de la cámara
  cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  // Ciclo de renderizado
  // -------------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Calcula la posición de la cámara
    direction = glm::vec3(0.0f);
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    process_input(
      window, 
      (glm::vec3*)glm::value_ptr(cameraPos), 
      (glm::vec3*)glm::value_ptr(cameraFront),
      (glm::vec3*)glm::value_ptr(cameraUp)
    );
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    matLocation = glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(model));

    view = glm::lookAt(
      cameraPos,
      cameraPos + cameraFront,
      cameraUp
    );
    matLocation = glGetUniformLocation(ourShader.ID, "view");
    glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(view));

    glBindVertexArray(VAO);
    drawCube(ourShader.ID);
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

void drawCube (unsigned int shaderId)
{
  unsigned int locationX, locationY, locationZ;
  locationX = glGetUniformLocation(shaderId, "invertX");
  locationY = glGetUniformLocation(shaderId, "invertY");
  locationZ = glGetUniformLocation(shaderId, "invertZ");

  glUniform1f(locationX, 1.0f);
  glUniform1f(locationY, 1.0f);
  glUniform1f(locationZ, 1.0f);

  // Draw right, top and front
  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

  // Draw left
  glUniform1f(locationX, -1.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glUniform1f(locationX, 1.0f);

  // Draw bottom
  glUniform1f(locationY, -1.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(int)));
  glUniform1f(locationY, 1.0f);

  // Draw back
  glUniform1f(locationZ, -1.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(int)));
  glUniform1f(locationZ, 1.0f);
}

void framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback (GLFWwindow *window, double xPos, double yPos)
{
  if (firstMouse)
  {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;
  lastX = xPos;
  lastY = yPos;

  xOffset *= SENSITIVITY;
  yOffset += SENSITIVITY;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f)
  {
    pitch = 89.0f;
  }
  if (pitch < -89.0f)
  {
    pitch = -89.0f;
  }
}

void process_input (GLFWwindow *window, glm::vec3 *cameraPos, glm::vec3 *cameraFront, glm::vec3 *cameraUp)
{
  float cameraSpeed = 2.5f * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    *cameraPos += *cameraFront * cameraSpeed;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    *cameraPos -= *cameraFront * cameraSpeed;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    *cameraPos -= glm::normalize(glm::cross(*cameraFront, *cameraUp)) * cameraSpeed;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    *cameraPos += glm::normalize(glm::cross(*cameraFront, *cameraUp)) * cameraSpeed;
  }
}