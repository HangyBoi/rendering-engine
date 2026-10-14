#pragma once

#include <utility>
#include <vector>
#include <glad/glad.h>
#include "vertex.h"

namespace core {
    // Owns its GPU buffers: created in the constructor, freed in the destructor.
    // One owner only: copy is deleted, move is allowed.
    class Mesh {

    public:

        // Constructor
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
        // Copy Constructor
        Mesh(const Mesh&) = delete;                 // copying to create a new Mesh is now a compiler error
        // Copy Assignment
        Mesh& operator=(const Mesh&) = delete;      // copying over an existing Mesh is now a compiler error
        // Move Constructor
        Mesh(Mesh&& other) noexcept;                // creating a new Mesh by taking from other
        // Move Assignment
        Mesh& operator=(Mesh&& other) noexcept;     // overwrite this Mesh by taking from other
        // Destructor
        ~Mesh();                                    // hand the ticket back

        void render() const;
        static Mesh generateQuad();

    private:

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        // Handles are just numbers pointing to GPU data. 0 = owns nothing.
        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;

        void setupBuffers();
    };
}