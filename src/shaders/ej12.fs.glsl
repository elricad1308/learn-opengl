#version 330 core

out vec3 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main ()
{
  float ambientStrength = 0.1f;
  vec3 ambientLight = lightColor * ambientStrength;

  FragColor = ambientLight * objectColor;
}