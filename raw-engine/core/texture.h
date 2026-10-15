#pragma once

#include <glad/glad.h>
#include <string>

namespace core {
    // Owns one GPU texture: created in the constructor, freed in the destructor.
    // One owner only: copy is deleted, move is allowed. If loading fails, id stays 0.
    class Texture {

    public:
        // Constructor: loads the image file and uploads it to the GPU.
        // explicit = no silent conversion from a string to a Texture.
        explicit Texture(const std::string& path);
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

        GLuint getId() const;

    private:
        // Handle (just a number). 0 = owns nothing.
        GLuint id = 0;
    };
}
