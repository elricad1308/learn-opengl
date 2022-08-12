#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main ()
{
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuseLight = diff * lightColor;

  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
  vec3 specularLight = specularStrength * spec * lightColor;

  float ambientStrength = 0.1;
  vec3 ambientLight = ambientStrength * lightColor;    
  FragColor = vec4((ambientLight + diffuseLight + specularLight) * objectColor, 1.0);
}