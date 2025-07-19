#version 330 core

in vec3 FragPos;
in vec2 texCoord;
//in vec3 oColor;
//in vec3 NormalColor;

out vec4 FragColor;

uniform sampler2D tex0;


void main()
{
   FragColor = texture(tex0, texCoord);
}