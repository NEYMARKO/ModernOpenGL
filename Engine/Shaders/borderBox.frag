#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lineColor;
uniform vec3 cameraPos;

void main()
{
    // Calculate the distance between the camera and the fragment
    float distanceToCamera = distance(FragPos, cameraPos);

    // Use smoothstep for a smoother transition
    float alpha = smoothstep(1/200.0, 1/5.0, 1/distanceToCamera);

    // Blend the line color with the background
    FragColor = vec4(lineColor, alpha);
}