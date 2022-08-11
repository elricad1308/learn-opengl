#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Cube
{
public:

  /**
   * @brief Construct a new Cube object
   * 
   * El cubo es inicializado transferiendo al GPU toda la info necesaria
   * para renderizar el cubo.
   */
  Cube ()
  {
    init();
  }

  ~Cube ()
  {
    clear();
  }

  /**
   * @brief Libera los recursos asociados a este cubo.
   * 
   * El método libera la memoria de todos los búfers inicializados de
   * este cubo.
   */
  void clear ()
  {
    if (VAO)
    {
      glDeleteVertexArrays(1, &VAO);
    }

    if (VBO)
    {
      glDeleteBuffers(1, &VBO);
    }

    if (EBO)
    {
      glDeleteBuffers(1, &EBO);
    }
  }

  /**
   * @brief Dibuja el cubo.
   * 
   * El método asocia el objeto de búfer del cubo (el cual contiene toda
   * la información del cubo), lo dibuja y después, desasocia el objeto
   * de búfer, para que éste no interfiera con otros objetos.
   */
  void draw ()
  {
    if (VAO)
    {
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
    else
    {
      std::cout << "The VAO is empty! Did you initialize the cube?" << std::endl;
    }
  }

  /**
   * @brief Inicializa los datos del cubo.
   * 
   * El método se encarga de crear los búfers en el GPU para almacenar
   * la información del cubo, además de transferirla al mismo.
   * 
   * Para evitar interferencia, tras completar la transferencia de datos
   * al GPU, todos los búfers asociados a este cubo se desasocían.
   */
  void init ()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  /**
   * @brief Get the Model matrix
   * 
   * @return glm::mat4 la matriz de modelo del cubo
   */
  glm::mat4 getModel ()
  {
    return model;
  }

  /**
   * @brief Set the Model matrix
   * 
   * @param _model la matriz de modelo del cubo
   */
  void setModel (glm::mat4 _model)
  {
    model = _model;
  }

  
private:
  /**
   * @brief Índices de los vértices
   * 
   * Estos índices definen el orden en que se deben dibujar los tríangu-
   * los que definen el cubo.
   */
  int indices[36] = {
    // Front
    0, 1, 3,
    1, 2, 3,

    // Back
    4, 5, 7,
    5, 6, 7,

    // Left
    0, 1, 4,
    0, 4, 5,

    // Right
    2, 3, 6,
    3, 6, 7,

    // Top
    1, 2, 6,
    1, 5, 6,

    // Bottom
    0, 4, 7,
    0, 3, 7
  };

  /**
   * @brief Coordenadas de los vértices del cubo.
   * 
   * Cada 3 valores definen una coordenada (X, Y, Z) en el espacio de
   * uno de los vértices del cubo. Combinando las 8 posiciones de los
   * vértices se pueden generar los 12 triángulos necesarios para dibu-
   * jar el cubo (2 tríangulos por cara, 6 caras en total).
   */
  float vertices[24] = {
    -0.5f, -0.5f,  0.5f,  // 0
    -0.5f,  0.5f,  0.5f,  // 1  1 2
     0.5f,  0.5f,  0.5f,  // 2  0 3
     0.5f, -0.5f,  0.5f,  // 3

    -0.5f, -0.5f, -0.5f,  // 4
    -0.5f,  0.5f, -0.5f,  // 5  5 6
     0.5f,  0.5f, -0.5f,  // 6  4 7
     0.5f, -0.5f, -0.5f   // 7
  };

  unsigned int EBO, VAO, VBO;

  glm::mat4 model;
};
#endif