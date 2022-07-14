#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
// uniform sampler2D texture2;

void main ()
{
  /*FragColor = mix(
    texture(texture1, vec2((TexCoord.x * 2.0f), (TexCoord.y * 2.0f))), 
    texture(texture2, vec2(TexCoord.x * 2.0f, TexCoord.y * 2.0f))
    , 0.2f
  ) * vec4(ourColor, 1.0f);*/
  FragColor = texture(texture1, vec2(TexCoord.x, TexCoord.y));// * vec4(ourColor, 1.0f);
}