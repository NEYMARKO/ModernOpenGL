#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 oColor;
//in vec3 NormalColor;

out vec4 FragColor;

//uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
    float ambientStrength = 0.1;
    //vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * oColor;

    vec3 result = (ambientStrength + diffuse) * oColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(NormalColor, 1.0);

}