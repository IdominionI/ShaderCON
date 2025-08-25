#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class oglfw_texture_class {
public:
    oglfw_texture_class() {}
    ~oglfw_texture_class() { Delete(); }

    //explicit Texture(const char *texturePath);
    void define_texture(const char* texturePath) {
        glGenTextures(1, &mID);
        glBindTexture(GL_TEXTURE_2D, mID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int texture_w;
        int texture_h;
        stbi_set_flip_vertically_on_load(true);
        if (stbi_uc* pixels = stbi_load(texturePath, &texture_w, &texture_h, nullptr, STBI_rgb_alpha)) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_w, texture_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<void*>(pixels));
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(pixels);
        }
    }

    void Bind() {
        glBindTexture(GL_TEXTURE_2D, mID);
    }

    void Delete() {
        glDeleteTextures(1, &mID);
    }

    void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint mID = 0;
};
