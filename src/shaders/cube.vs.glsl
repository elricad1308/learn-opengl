#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float invertX;
uniform float invertY;
uniform float invertZ;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main ()
{
  gl_Position = projection 
    * view 
    * model 
    * vec4(aPos.x * invertX, aPos.y * invertY, aPos.z * invertZ, 1.0f);
  TexCoord = aTexCoord;
}