#pragma once

#include <glad/glad.h>
#include <string>

namespace core {

    class Texture {
    private:
        GLuint id = 0;

    public:
        Texture(const std::string& path);

        GLuint getId() const;

        // Copy Constructor
        Texture(const Texture&) = delete;
        // Copy Assignment
        Texture& operator=(const Texture&) = delete;
        // Move Constructor
        Texture(Texture&& other) noexcept;
        // Move Assignment
        Texture& operator=(Texture&& other) noexcept;
        // Destructor
        ~Texture();
    };
}
