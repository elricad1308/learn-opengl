#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main ()
{
  float ambientStrength = 0.1;
  vec3 ambientLight = ambientStrength * lightColor;    
  FragColor = vec4(ambientLight * objectColor, 1.0);
}