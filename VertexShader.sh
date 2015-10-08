#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 ourColor;
out vec2 Texcoord;


void main()
{
    gl_Position = projection * view * model * vec4(position + offset, 1.0f);
    Texcoord = texcoord;
    ourColor = color;
}
