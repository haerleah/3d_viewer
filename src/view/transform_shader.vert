#version 330 core

layout(location = 0) in vec3 vertex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
}

