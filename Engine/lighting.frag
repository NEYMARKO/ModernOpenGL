#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0);  // Assuming ocolor is RGB, you might need to adjust if it's not.
}