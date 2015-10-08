#version 420 core
in vec4 ourColor;
in vec2 Texcoord;

out vec4 color;

uniform sampler2D uiSampler;

void main()
{
    color = ourColor * texture(uiSampler, Texcoord);

}
