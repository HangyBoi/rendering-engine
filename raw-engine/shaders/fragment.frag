#version 400 core
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
in vec3 fragPosWorld;
in mat3 TBN;

uniform sampler2D normalMap;
uniform vec3 viewPosition;

void main()
{
   vec3 normal = normalize(texture(normalMap, uv).rgb * 2.0 - 1.0);     // already tangent space
   vec3 fragPos = TBN * fragPosWorld;
   vec3 viewDir = normalize(TBN * viewPosition - fragPos);

   // debug: show the normal map's normal as a colour
   FragColor = vec4(normal * 0.5 + 0.5, 1.0);

   //FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}