#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/shader_s.h"

/**
 * Carga la librería stb_image.h
 * 
 * Esta librería permite cargar los formatos de imágenes más populares
 * de manera fácil.
 */
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void framebuffer_size_callback (GLFWwindow *window, int width, int height);
void process_input (GLFWwindow *window);

int main ()
{
  // Variables
  int width, height, channels;
  int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  float vertex_data[] = {
    // Posiciones        // Colores         // Coords. Textura
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Inferior izq
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Superior izq
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Superior der
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f  // Inferior der
  };
  unsigned char *data;
  unsigned int texture, texture2, EBO, VAO, VBO;
  GLFWwindow *window;

  // Inicialización
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
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
    std::cout << "Error al inicializar GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // Genera un objeto textura y lo vincula para trabajar con él
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Configura el modo en el que se aplicará la textura. Las texturas
  // utilizan los ejes s, t y r de manera análoga a los ejes coordinados
  // x, y, z.
  // OpenGL ofrece varios modos para aplicar una textura:
  // - GL_REPEAT: el modo por defecto: la textura se repite para llenar
  //   el objeto.
  // - GL_MIRRORED_REPEAT: similar a GL_REPEAT, pero la textura se 
  //   refleja (horizontal y/o verticalmente) cada repetición
  // - GL_CLAMP_TO_EDGE: la textura se mapea al espacio normal en el
  //   intervalo (0, 1), y cualquier coordenada fuera de este rango
  //   mostrará el borde de la textura estirado para cubrirla.
  // - GL_CLAMP_TO_BORDER: similar a GL_CLAMP_TO_EDGE, pero en lugar de
  //   estirar el borde de la textura, se mostrará un color de borde
  //   definido por el usuario.
  // Estas opciones puede aplicarse de manera individual a cada eje  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Especifica el modo en cómo debe hacerse el filtrado de textura
  // Una textura en OpenGL, independientemente de su tamaño, se mapea en
  // un espacio coordenado normalizado de (0, 1) en los ejes s y t (y
  // r, para el caso de texturas 3D). OpenGL debe decidir cuál pixel de
  // la textura (conocido también como 'texel') corresponde a cada coor-
  // denada dentro del intervalo, proceso conocido como filtrado de tex-
  // tura (Texture Filtering).
  // Existen varios métodos de filtrado de textura, pero dos son los más
  // usados:
  // - GL_NEAREST: el método por defecto: OpenGL usa el texel cuyo cen-
  //   tro esté más cercano a la coordenada de la textura.
  // - GL_LINEAR: OpenGL toma un muestreo de los texels cercanos a la
  //   coordenada, y produce como resultado un pixel con los valores
  //   ponderados de ellos; mientras menor sea la distancia del centro
  //   de un texel a la coordenada de la textura, mayor será la contri-
  //   bución de dicho texel al color final.
  // El filtrado de textura se puede aplicar cuando el objeto es más
  // pequeño que la textura (MINifying), o si éste es más grande
  // (MAGnifying) de manera independiente. El filtrado MIN también
  // puede indicar cómo se aplicarán los Mipmaps (explicados más adelan-
  // te). El filtrado MAG no debe especificar cómo procesar los Mipmaps,
  // dado que éstos solo se usan cuando el objeto es más pequeño que la
  // textura; hacerlo puede resultar en un error de OpenGL
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Carga de la textura
  // El primer argumento es la ruta a la imagen con la textura. la li-
  // brería cargará la imagen, y establecerá su ancho, alto y número
  // de canales de color en las variables width, height y channels.
  data = stbi_load("../../textures/container.jpg", &width, &height, &channels, 0);
  if (data)
  {
    // Transfiere la imagen al objeto textura  
    glTexImage2D(
      // Previamente vinculamos la textura a la bandera GL_TEXTURE_2D, 
      // por lo que al transferir los datos a esta bandera, se almacena-
      //rán en // nuestro objeto
      GL_TEXTURE_2D, 

      // Nivel de mipmap. Los mipmaps son copias de la textura con
      // dimensiones progresivamente menores, usadas por OpenGL para 
      // aplicarlas a objetos que se encuentren a mayor distancia (y por
      // tanto, son más pequeños) sin que éstos pierdan detalles. Esta
      // textura es la inicial, por lo tanto usa el nivel de mipmap 0 
      // (el original).
      0, 
    
      // Formato en el que se almacenará la textura. La imagen de la 
      // textura usa el canal RGB, por lo que la guardamos de la misma
      // forma.
      GL_RGB, 
    
      // Dimensiones de la textura, éstas fueron calculadas automática-
      // mente por la librería stb_image al cargar la imagen
      width, height, 
    
      // Valor legado, debe ser siempre cero
      0, 
    
      // Formato en el que se encuentra la imagen fuente. stb_image la 
      // cargó en formato RGB y la almacenó como char sin signo, por lo
      // que indicamos a OpenGL la forma en la que debe interpretar la
      // textura
      GL_RGB, GL_UNSIGNED_BYTE, 

      // Datos de la textura, cargados por stb_image
      data
    );

    // Genera el mipmap de nuestra textura. Como se explicó anteriormen-
    // te, los mipmaps son copias progresivamente más pequeñas de la 
    // textura, usadas para aplicarse a objetos que se encuentren lejos 
    // para que no pierdan detalles y, además, se ahorren recursos. Los 
    // mipmaps se generarán reduciendo la textura a la mitad por cada 
    // nivel, hasta que la imagen tenga una dimensión de 1 pixel.
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Error al cargar la textura" << std::endl;
  }

  // Libera la imagen de la memoria: ésta ya se encuentra en el objeto
  // Textura y no se requiere tenerla abierta
  stbi_image_free(data);

  // Carga la segunda textura
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Invierte la coordenada Y de la textura
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("../../textures/awesomeface.png", &width, &height, &channels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

  // Buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Posición
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Coordenadas de textura
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Vincula a cada uniform de textura con su respectivo canal
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activa los canales de textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Limpieza
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
  ourShader.clear();
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