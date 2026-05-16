R"=====(#version 420 core
out vec4 FragColor;

in vec4 VertColor;
in vec2 VertUV;

layout (binding = 0) uniform sampler2D Texture;

void main()
{
    FragColor = VertColor * texture(Texture, VertUV);
} )====="