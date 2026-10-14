#include "shader.h"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <utility>
#include <glm/gtc/type_ptr.hpp>

namespace core {
    // Compile both files, link them into one program.
    // On any error: print why and leave program = 0.
    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

        GLuint vs = compile(GL_VERTEX_SHADER, vertexPath);
        GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentPath);
        if (vs == 0 || fs == 0) {
            // OpenGL ignores glDeleteShader(0), so this is safe even if only one failed
            glDeleteShader(vs);
            glDeleteShader(fs);
            printf("Error compiling shader\n");
            return;
        }

        GLint success = 0;
        char infoLog[512];
        program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            printf("Error! Making Shader Program: %s\n", infoLog);
            glDeleteProgram(program);
            program = 0;
        }

        // The program keeps its own compiled code, so the two shader objects can go
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    // Move constructor: take other's program and cache, leave other empty (0).
    Shader::Shader(Shader&& other) noexcept {
        uniformCache = std::move(other.uniformCache);
        program = other.program;
        other.program = 0;
    }

    // Move assignment: free our old program first, then take other's.
    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (program != 0) glDeleteProgram(program);
            program = other.program;
            other.program = 0;

            // The cache must follow the program: slot numbers differ between programs
            uniformCache = std::move(other.uniformCache);
        }
        return *this;
    }

    // Hand the program back to OpenGL
    Shader::~Shader() {
        if (program != 0) {
            glDeleteProgram(program);
        }
    }

    void Shader::use() const {
        glUseProgram(program);
    }

    bool Shader::isValid() const {
        return program != 0;
    }

    // glUniform* writes to the ACTIVE program, so call use() first.
    // Arguments: slot, how many matrices, don't transpose, address of the 16 floats.
    void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    GLint Shader::getUniformLocation(const std::string& name) const {
        // Asked before? Return the saved slot.
        auto it = uniformCache.find(name);
        if (it != uniformCache.end()) {
            return it->second;
        }

        // First time: ask OpenGL once. Save even -1, so we don't ask (or warn) again every frame.
        GLint location = glGetUniformLocation(program, name.c_str());
        if (location == -1) {
            printf("Could not find uniform location: %s\n", name.c_str());
        }
        uniformCache[name] = location;
        return location;
    }

    std::string Shader::readFile(const std::string &path) {
        std::ifstream fileStream(path, std::ios::in);
        if (!fileStream.is_open()) {
            printf("Could not open file: %s\n", path.c_str());
            return "";
        }
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        return buffer.str();
    }

    GLuint Shader::compile(GLenum shaderType, const std::string &path) {
        printf("Loading shader: %s\n", path.c_str());

        const std::string shaderText = readFile(path);
        // Empty text = file missing (readFile already printed why)
        if (shaderText.empty()) return 0;
        const GLuint shader = glCreateShader(shaderType);
        const char *s_str = shaderText.c_str();

        glShaderSource(shader, 1, &s_str, nullptr);
        glCompileShader(shader);
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("Error! Shader issue [%s]: %s\n", path.c_str(), infoLog);
            // Free the half-made shader, otherwise it leaks
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }
}
