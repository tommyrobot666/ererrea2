R"=====(#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 VertColor;
out vec2 VertUV;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    VertColor = vec4(aColor, 1f);
    VertUV = aTexCoord;
})====="