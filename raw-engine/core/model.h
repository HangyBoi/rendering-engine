#pragma once

#include <utility>
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"

namespace core {
    class Model {
    private:
        std::vector<core::Mesh> meshes;
        glm::mat4 modelMatrix;
    public:
        // std::move: take the meshes, don't copy (Mesh can't be copied).
        // No destructor/move code needed here: the vector handles it ("rule of zero").
        Model(std::vector<core::Mesh> meshes) : meshes(std::move(meshes)), modelMatrix(1) {}

        void render() const;

        void translate(glm::vec3 translation);
        void rotate(glm::vec3 axis, float radians);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
    };
}