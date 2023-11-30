#version 330 core

in vec3 ocolor;  // input variable from the vertex shader

out vec4 FragColor;

void main()
{
    FragColor = vec4(ocolor, 1.0);  // Assuming ocolor is RGB, you might need to adjust if it's not.
}