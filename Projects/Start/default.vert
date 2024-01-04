#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
//out vec3 NormalColor; // New output variable for color visualization

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);


    // Convert normalized normal to RGB color
    //NormalColor = 0.5 * Normal + 0.5; // Scale and bias to bring values to [0, 1] range
}