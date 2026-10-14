#include "shader.h"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <glm/gtc/type_ptr.hpp>

namespace core {
    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

        GLuint vs = compile(GL_VERTEX_SHADER, vertexPath);
        GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentPath);
        if (vs == 0 || fs == 0) {
            glDeleteShader(vs);
            glDeleteShader(fs);
            printf("Error compiling shader\n");
            return;
        }

        int success;
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

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader::Shader(Shader&& other) noexcept {
        uniformCache = std::move(other.uniformCache);
        program = other.program;
        other.program = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (program != 0) glDeleteProgram(program);
            program = other.program;
            other.program = 0;

            uniformCache = std::move(other.uniformCache);
        }
        return *this;
    }

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

    void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    GLint Shader::getUniformLocation(const std::string& name) const {
        auto it = uniformCache.find(name);
        if (it != uniformCache.end()) {
            return it->second;
        }

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
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }
}
