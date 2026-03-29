R"=====(#version 420 core
out vec4 FragColor;

in vec4 VertColor;
in vec2 VertUV;

layout (binding = 0) uniform sampler2D Texture;

void main()
{
    FragColor = vec4(.3f,.7f,.2f,1f);// VertColor * texture(Texture, VertUV);
} )====="