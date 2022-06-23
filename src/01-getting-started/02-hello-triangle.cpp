#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback (GLFWwindow* window, int width, int height);
void processInput (GLFWwindow* window);

/**
 * @brief Shader de vértices.
 * 
 * Los 'shaders' son pequeños programas que se ejecutan directamente en
 * el GPU. El shader de vértices (Vertex Shader) es el primer paso del
 * proceso que sigue OpenGL para renderizar gráficos: básicamente se
 * encarga de tomar la información de UN vértice (en este caso, repre-
 * sentado por el vector de tamaño 3 aPos) para transformar sus coorde-
 * nadas al espacio 3D NDC (coordenadas normalizadas de dispositivo).
 * 
 * En nuestro ejemplo, como la información de los vértices ya se encuen-
 * tra normalizada, el shader únicamente copia la coordenada del vértice
 * a la variable predefinida gl_Position, que representa la localización
 * del vértice en la pantalla.
 */
const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

/**
 * @brief Shader de fragmentos.
 * 
 * El shader de fragmentos (Fragment Shader) es el quinto paso del
 * proceso que sigue OpenGL para renderizar gráficos. Éste toma los
 * 'fragmentos' producidos por el paso anterior (Rasterización) y
 * se encarga de determinar el color que dicho fragmento debe tener.
 * 
 * Un 'fragmento' es toda la información que OpenGL necesita para rende-
 * rizar un único pixel. Este shader tiene información acerca de la
 * escena (por ejemplo, posición y color de la luz) para determinar el
 * color que dicho pixel debe tener en la pantalla.
 * 
 * En este ejemplo, para mantener las cosas simples. el Fragment Shader
 * siempre muestra los pixeles con un color naranja, expresado en un
 * vector4 que representa los canales RGBA (rojo, verde, azul y alpha).
 */
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

int main ()
{
  /**
   * Coordenadas del triángulo a dibujar.
   * 
   * OpenGL sólo procesa coordenadas 3D en el rango (-1.0, 1.0) en los
   * tres ejes coordenados (X, Y, Z), llamadas Coordenadas Normalizadas
   * de Dispositivo (normalized device coordinates). Cualquier coordena-
   * da fuera de dicho rango no será visible en la pantalla.
   * 
   * TODO en OpenGL se encuentra en un espacio matemático 3D, por lo que,
   * incluso si queremos dibujar un triángulo en 2D, necesitamos indicar
   * la posición en Z; en este caso, manteníendola igual en cero, lo
   * cual da la apariencia de ser un 2D.
   */
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // -------------------------------------------------------------------
  // Inicialización de glfw (explicado en el programa anterior)
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Error al crear una ventana GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Falló la inicialización de GLAD" << std::endl;
    glfwTerminate();
    return -1;    
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // Fin del código de inicialización
  // -------------------------------------------------------------------

  // Crea un objeto Vertex Shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Agrega el código del Vertex Shader definido al principio al obje-
  // to recién creado, para ser compilado en tiempo de ejecución.
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Obtiene información acerca del resultado de la compilación del
  // shader, para ayudar a diagnosticar errores.
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Crea un objeto Fragment Shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Agrega el código del Fragment Shader definido al principio al obje-
  // to recién creado, para ser compilado en tiempo de ejecución.
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Obtiene información acerca del resultado de la compilación del
  // shader, para ayudar a diagnosticar errores.
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  
  // Crea un Program Shader
  // Los Program Shader son la versión final de múltiples shader combi-
  // nados, donde cada Shader de un paso recibe el resultado del Shader
  // del paso anterior.
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  // Agrega los shaders que creamos previamente al Shader Program
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Revisamos el estado de compilación del Shader Program
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Activa el programa para ser usado
  glUseProgram(shaderProgram);

  // Una vez que los shaders han sido incorporados al programa, ya no
  // son necesarios y pueden ser eliminados para liberar recursos.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  /**
   * Genera un objeto de buffer de vértices (Vertex Buffer Object).
   * 
   * Los programas se ejecutan en el CPU, mientras que OpenGL se ejecuta
   * en el GPU. Mover datos del CPU al GPU es un proceso lento, por lo
   * cual es deseable enviar la mayor cantidad de datos en lote al GPU,
   * dónde se procesarán extremadamente rápido. Los VBO permiten reser-
   * var espacios de memoria del GPU que recibirán la información de los
   * vértices a procesar, reduciendo de esta forma el cuello de botella
   * que es la transferencia de datos CPU <-> GPU.
   */
  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);

  // Todo este proceso debe repetirse para cada elemento que se dibuje.
  // Nuestro ejemplo es un simple triangulo y aun así, es bastante largo
  // así que imagina el trabajo necesario para dibujar una escena con
  // cientos de elementos (lo cual no es raro). Por ello, OpenGL provee
  // de un mecanismo que permite "guardar" la información de un elemento
  // para ser utilizado cada vez que se quiera dibujar: los Vertex Array
  // Objects (VAO). De esta forma, creamos un VAO con la información de
  // nuestro triángulo, y cada que queramos dibujarlo, simplemente en-
  // viamos el VAO a OpenGL y él se encargará del resto.  
  glGenVertexArrays(1, &VAO);

  // Vincula el VAO para ser transferido al GPU.
  glBindVertexArray(VAO);

  // OpenGL permite reservar memoria en el GPU para varios buffers a la
  // vez, pero solo uno de cada tipo. Los VBO son tipo GL_ARRAY_BUFFER.
  // Una vez reservado, cualquier llamada que se haga al buffer de tipo
  // GL_ARRAY_BUFFER utilizará el buffer reservado, VBO.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Transfiere la información de los vértices al buffer reservado (es
  // decir, al VBO que, a su vez, lo copia en el GPU). El segundo argu-
  // mento es la cantidad de información (en bytes) a transferir, mien-
  // tras que el tercer argumento es la información a copiar.
  // El cuarto y último argumento indica la manera en la que el GPU debe
  // procesar los datos:
  // - GL_STREAM_DRAW  : los datos se envían una vez, y el GPU los usará
  //    pocas veces.
  // - GL_STATIC_DRAW  : los datos se envían una vez y se usan muchas 
  //   veces.
  // - GL_DYNAMIC_DRAW : los datos cambiarán constantemente y se usarán
  //   muchas veces.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Los shader de vertex nos permiten introducir la información de los
  // vértices de cualquier forma que deseemos: esto permite una gran
  // flexibilidad, pero también significa que debemos especificar a
  // OpenGL el formato que tienen nuestros vértices.
  // En nuestro ejemplo, los vértices tienen estas características:
  //   - Cada dato de posición está almacenado como valores float de
  //     32 bits (4 bytes)
  //   - Cada coordenada está compuesta por 3 de esos valores (12 bytes)
  //   - No hay ningún espacio (u otros valores) entre cada coordenada
  //   - La primera coordenada se encuentra el inicio del buffer
  // Los atributos de vertex toman los datos desde la memoria reservada
  // por un VBO, y dado que anteriormente creamos y vinculamos un VBO
  // al apuntador GL_ARRAY_BUFFER, al momento de ejecutar esta función
  // los datos de los vértices son tomados desde dicho VBO
  glVertexAttribPointer(
    0     // Índice del atributo del vértice que queremos modificar.
          // En el shader, definimos (location = 0), lo que hace que la
          // posición del vértice se encuentre en dicho índice (cero).

    , 3   // Tamaño del atributo del vértice, en este caso, las coorde-
          // nadas en X, Y, Z

    , GL_FLOAT  // Tipo de dato de la información del atributo

    , GL_FALSE  // Indica si queremos normalizar los datos. Nuestros
                // ya estan normalizados, por lo que no necesitamos ha-
                // cerlo de nuevo.

    , 3 * sizeof(float) // Tamaño del  "paso", o espacio entre atributos
                        // de vértices consecutivos. En este caso, cada
                        // 3 floats empieza un nuevo atributo de vértice
                        // (una nueva posición)

    , (void*)0  // Offset, o posición a partir del inicio en donde co-
                // mienzan los atributos de los vértices. Como están al
                // principio del buffer (posición 0) y la función re-
                // quiere un apuntador void*, es que se hace el casting.
  );

  // 'Activa' los atributos de vértices en el índice cero (es decir, las
  // posiciones). Por defecto, los atributos de vértice están DESACTIVA-
  // DOS y tienen que activarse manualmente.
  glEnableVertexAttribArray(0);

  // Una vez que la copia de datos ha terminado, el GL_ARRAY_BUFFER pue-
  // de desvincularse de manera segura
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Desvincula el VAO para que otras llamadas no lo modifiquen de mane-
  // ra accidental
  glBindVertexArray(0);

  // Ciclo de renderizado
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibuja el triángulo
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Intercambia buffers y procesa eventos I/O
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Libera recursos del GPU antes de salir
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // Libera recursos del CPU antes de salir
  glfwTerminate();
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
void framebuffer_size_callback (GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

/**
 * @brief Procesa las entradas del usuario.
 * 
 * @param window el objeto con el contexto de la ventana.
 */
void processInput (GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}
