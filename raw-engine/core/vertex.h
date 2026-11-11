#pragma once


#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent) : position(position), normal(normal), uv(uv), tangent(tangent) {}
};