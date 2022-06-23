#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback (GLFWwindow* window, int width, int height);

void processInput (GLFWwindow* window);

/**
 * @brief Enseña como configurar un proyecto OpenGL usando GLFW y GLAD.
 * 
 * GLFW (Graphics Library Framework) es una librería que ayuda a crear
 * ventanas y contextos de OpenGL, además de procesar entradas del usua-
 * rio como pulsaciones del teclado, ratón o incluso joysticks. Gracias
 * a ella, los programadores pueden olvidarse del procedimiento necesa-
 * rio para crear la ventana de aplicación en los diferentes sistemas
 * operativos y entornos de escritorio (Windows, OS X, X11 y Wayland) y
 * concentrarse en la programación gráfica.
 * 
 * Por otro lado, dado que OpenGL es solamente una especificación y es
 * tarea de los fabricantes de tarjetas gráficas implementar las funcio-
 * nes que lleven a cabo dicha especificación, la ubicación de los dife-
 * rentes archivos y librerías varía de fabricante a fabricante, y es
 * necesario conocer su localización para poder almacenar punteros a las
 * funciones que se requieran. Este proceso, además de tardado, es de-
 * pendiente del sistema operativo, lo cual representa aún más trabajo 
 * para el desarrollador. Afortunadamente, existe Glad, una librería que
 * se encarga de realizar el trabajo anterior descrito de forma automá-
 * tica y fácil.
 * 
 * @return int 
 */
int main ()
{
  // Inicializa la librería GLFW, que nos permite utilizar las funciones
  // glfw*
  glfwInit();

  // Configura la ventana creada por GFWL para usar la versión 3.3 de
  // OpenGL, en modo Core-Profile (lanza errores si se usan funciones
  // del modo inmediato)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Crea el objeto Window, que permite configurar las opciones de la
  // ventana de la aplicación.
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Error al crear la ventana GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Establece la ventana recién creada como el contexto principal
  glfwMakeContextCurrent(window);

  // Asegura que la librería Glad se haya cargado correctamente antes de
  // empezar a utilizar las funciones de OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Error al inicializar GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Establece el tamaño del espacio de renderizado igual al de la 
  // ventana que creamos previamente.
  glViewport(0, 0, 800, 600);

  // Registra el callback para redimensionar el área de renderizado
  // cuando el usuario cambie el tamaño de la ventana.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Ciclo de renderizado: este ciclo mantiene la ventana activa hasta
  // que se cierre, además de procesar las entradas del usuario.
  while (!glfwWindowShouldClose(window))
  {
    // Establece el color de limpieza a azul turquesa. Esta es un ejem-
    // plo de función que establece el contexto de OpenGL
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Limpia la pantalla.
    // Si la pantalla no se limpia, el frame previo quedaría en la pan-
    // talla, y el frame actual se dibujaría encima, combinando el re-
    // sultado de ambos. Esta es un ejemplo de función que usa el con-
    // texto de OpenGL (establecido por la función anterior).
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Procesa las entradas del usuario
    processInput(window);

    // Revisa si algún evento fue activado (por ejemplo, pulsaciones del
    // teclado o movimientos del ratón).
    glfwPollEvents();

    // Cambia el back buffer con el front buffer. Para un mejor rendi-
    // miento y una experiencia más fluida, OpenGL procesa el frame
    // "siguiente" en un buffer alterno mientras la ventana muestra el
    // contenido del buffer actual: esta función mueve el contenido del
    // back buffer al front buffer (que es el que despliega la ventana)
    // para iniciar con el procesamiento del frame siguiente.
    glfwSwapBuffers(window);
  }

  // Libera los recursos de la aplicación antes de salir.
  glfwTerminate();

  return 0;
}

/**
 * @brief ajusta el área de renderizado al redimensionar la ventana.
 * 
 * Este callback es llamado cada vez que el usuario cambia el tamaño de
 * la ventana de la aplicación, para ajustar el área de renderizado de
 * OpenGL de manera acorde.
 * 
 * @param window el objeto con el contexto de la ventana.
 * 
 * @param width la nueva anchura de la ventana, en pixeles.
 * 
 * @param height la nueva altura de la ventana, en pixeles.
 */
void framebuffer_size_callback (GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

/**
 * @brief procesa las entradas del usuario.
 * 
 * Este callback es llamado una vez durante cada repetición del ciclo
 * de renderizado, mejor conocida como 'frame', y procesa las entradas 
 * del usuario, en este caso, detecta si se pulsó la tecla ESC para 
 * cerrar la aplicación.
 * 
 * @param window el objeto con el contexto de la ventana.
 */
void processInput (GLFWwindow* window)
{
  // Determina si la tecla ESC fue presionada
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    // Activa el atributo 'ShouldClose', lo que finalizará el ciclo de
    // renderizado en el siguiente frame.
    glfwSetWindowShouldClose(window, true);
  }
}