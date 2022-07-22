#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aCol;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
// out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main ()
{
  // La multiplicación de matrices se lee de derecha a izquierda, por
  // tanto primero se aplica la transformación de modelo, luego la
  // de vista y de último la de proyección
  gl_Position = projection * view * model * vec4(aPos, 1.0f);

  // ourColor = aCol;
  TexCoord = aTexCoord;
}