#pragma once

#include <string>
#include <glad/glad.h>
#include <unordered_map>
#include <glm/ext/matrix_float4x4.hpp>

namespace core {
    // Owns one OpenGL shader program (a vertex + fragment file, compiled and linked).
    // Created in the constructor, freed in the destructor. One owner only: copy is deleted, move is allowed.
    // If compiling or linking fails the program stays 0, so check isValid().
    class Shader {

    public:
        // Constructor: reads, compiles and links both files
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        // Copy Constructor
        Shader(const Shader&) = delete;
        // Copy Assignment
        Shader& operator=(const Shader&) = delete;
        // Move Constructor
        Shader(Shader&& other) noexcept;
        // Move Assignment
        Shader& operator=(Shader&& other) noexcept;
        // Destructor
        ~Shader();

        // Makes this the active program. Call it before the setters.
        void use() const;
        // False if compiling or linking failed
        bool isValid() const;
        // Send a value to the uniform with this name (in the active program)
        void setMat4(const std::string& name, const glm::mat4& value) const;
        void setInt(const std::string& name, int value) const;

    private:
        // Handle (just a number). 0 = owns nothing.
        GLuint program = 0;
        // Uniform name -> slot number. Asking OpenGL is slow, so each answer is saved here.
        // 'mutable' lets the const setters fill it.
        mutable std::unordered_map<std::string, GLint> uniformCache;

        // Slot number of a uniform (-1 = not found). Uses the cache.
        GLint getUniformLocation(const std::string& name) const;
        // File text, or "" if the file can't be opened
        static std::string readFile(const std::string& path);
        // Compiles one shader file. Returns its handle, or 0 on error.
        static GLuint compile(GLenum type, const std::string& path);

    };
}