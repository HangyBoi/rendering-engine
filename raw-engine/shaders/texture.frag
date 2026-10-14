#version 400 core
out vec4 FragColor;
in vec3 fNor;
in vec2 uv;
uniform sampler2D diffuseTexture;

void main()
{
    vec4 diffuse = texture(diffuseTexture, uv);
    FragColor = vec4(diffuse.r, diffuse.g, diffuse.b, 1);
}