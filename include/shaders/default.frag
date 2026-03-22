R"=====(#version 330 core
out vec4 FragColor;

in vec4 VertColor;
in vec2 VertUV;

uniform sampler2D Texture;

void main()
{
    FragColor = VertColor * texture(Texture, VertUV);
} )====="