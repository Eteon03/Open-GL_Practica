#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm.hpp>
#include "../Headers/stb_image.h"

class Heightmap {
public:
    Heightmap(const std::string& heightmap_path, float width, float depth, float max_height);
    ~Heightmap();

    void render();

private:
    GLuint vao_id;
    GLuint vbo_id;
    GLuint ebo_id;
    GLuint texture_id; 

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLuint> indices;

    int rows, cols; // Dimensiones del heightmap

    void load_heightmap(const std::string& path, float max_height);
    void generate_mesh(float width, float depth);
};
