#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTan;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec3 fPos;
out vec3 fNor;
out vec2 uv;
out vec3 fragPosWorld;
out mat3 TBN;

void main() {
  fPos = aPos;
  fNor = aNor;
  uv = aUv;
  fragPosWorld = vec3(modelMatrix * vec4(aPos, 1.0));

  vec3 N = normalize(mat3(modelMatrix) * aNor);
  vec3 T = normalize(mat3(modelMatrix) * aTan);
  vec3 B = normalize(cross(N, T));
  TBN = transpose(mat3(T, B, N));

  gl_Position = mvpMatrix * vec4(aPos, 1.0);
}