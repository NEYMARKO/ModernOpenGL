#version 330 core

in vec3 FragPos;
in vec2 texCoord;
//in vec3 oColor;
//in vec3 NormalColor;

out vec4 FragColor;

uniform sampler2D tex0;


void main()
{
   vec4 texColor = texture(tex0, texCoord);
   if (texColor.a < 0.1) discard;
   FragColor = texColor;
}