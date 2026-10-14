#include "mesh.h"

namespace core {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) : vertices(std::move(vertices)), indices(std::move(indices)) {
        setupBuffers();
    }

    void Mesh::setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(Vertex) * vertices.size()), &vertices[0],
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(unsigned int) * indices.size()),
                     &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));
        glBindVertexArray(0);
    }

    Mesh Mesh::generateQuad() {
        const glm::vec3 pos[] = {
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f),
        };

        const glm::vec3 normals[] = {
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
        };
        const glm::vec2 uvs[] = {
                glm::vec2(0, 0),
                glm::vec2(1, 0),
                glm::vec2(0, 1),
                glm::vec2(1, 0),
                glm::vec2(1, 1),
                glm::vec2(0, 1)
        };
        const std::vector<GLuint> indices = {0, 1, 2, 3, 4, 5};

        std::vector<Vertex> vertexVector;
        vertexVector.reserve(6);
        for (int i = 0; i < 6; ++i) {
            vertexVector.emplace_back(pos[i], normals[i], uvs[i]);
        }

        return Mesh(vertexVector, indices);
    }

    // Move constructor: take other's handles. Nothing to free first, this Mesh is new.
    Mesh::Mesh(Mesh&& other) noexcept {
        // Take the other object’s handles.
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        // Set the other object’s handles to 0.
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;

        // Move the vectors too: takes their contents instead of copying.
        vertices = std::move(other.vertices);
        indices  = std::move(other.indices);
    }

    // Move assignment: same, but free my old buffers first.
    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        // Skip self-assignment (a = std::move(a)), or we would free our own data.
        if (this != &other) {
            // Free the old buffers before overwriting the handles, or they leak.
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            VAO = other.VAO;
            other.VAO = 0;
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            VBO = other.VBO;
            other.VBO = 0;
            if (EBO != 0) glDeleteBuffers(1, &EBO);
            EBO = other.EBO;
            other.EBO = 0;

            vertices = std::move(other.vertices);
            indices  = std::move(other.indices);
        }
        return *this;
    }

    // Free the GPU buffers. A moved-from Mesh holds 0s, so it frees nothing.
    // Needs a live OpenGL context: must run before glfwTerminate().
    Mesh::~Mesh() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
    }

    void Mesh::render() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }
}