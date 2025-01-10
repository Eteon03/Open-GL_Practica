#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm.hpp>
#include "../Headers/stb_image.h"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();
    void set_texture(GLuint texture_id);
    GLuint get_texture_id();
    void render();

private:
    GLuint vao_id;
    GLuint vbo_id;
    GLuint texture_id;
    std::vector<std::string> faces;

    void load_textures();
    void setup_buffers();
};
