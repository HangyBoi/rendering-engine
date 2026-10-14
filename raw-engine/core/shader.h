#pragma once

#include <string>
#include <glad/glad.h>
#include <unordered_map>
#include <glm/ext/matrix_float4x4.hpp>

namespace core {
    class Shader {

        public:
        // Constructor
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

        void use() const;
        bool isValid() const;
        void setMat4(const std::string& name, const glm::mat4& value) const;
        void setInt(const std::string& name, int value) const;

    private:
        GLuint program = 0;
        mutable std::unordered_map<std::string,GLint> uniformCache;

        GLint getUniformLocation(const std::string& name) const;
        static std::string readFile(const std::string& path);
        static GLuint compile(GLenum type, const std::string& path);

    };
}