
#pragma once
#include <string>
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

class Texture {
public:
    Texture(const std::string& file_path);
    ~Texture();

    void bind(unsigned int unit = 0) const;
    void unbind() const;

private:
    GLuint texture_id;
    int width, height, channels;
};