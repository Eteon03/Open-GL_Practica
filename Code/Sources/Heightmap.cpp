#include "../Headers/Heightmap.hpp"
#include <iostream>

Heightmap::Heightmap(const std::string& heightmap_path, float width, float depth, float max_height) {
    // Cargar el heightmap y generar la malla
    load_heightmap(heightmap_path, max_height);
    generate_mesh(width, depth);

    // Crear y configurar buffers de OpenGL
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);
    glGenBuffers(1, &ebo_id);

    glBindVertexArray(vao_id);

    // Enviar datos de vértices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2); // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    // Enviar datos de índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Heightmap::~Heightmap() {
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ebo_id);
}

void Heightmap::load_heightmap(const std::string& path, float max_height) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Error: No se pudo cargar el heightmap desde " << path << std::endl;
        return;
    }

    rows = height;
    cols = width;

    // Reservar memoria para los vectores
    vertices.resize(rows * cols * 8); // Cada vértice tiene posición (3), normal (3) y UV (2)
    normals.resize(rows * cols * 3); // Cada normal tiene 3 componentes
    uvs.resize(rows * cols * 2);     // Cada UV tiene 2 componentes

    for (int z = 0; z < rows; ++z) {
        for (int x = 0; x < cols; ++x) {
            int index = (z * cols + x) * channels;
            float intensity = data[index] / 255.0f;
            float y = intensity * max_height;

            // Calcular índices
            int vertex_index = (z * cols + x) * 8;
            int normal_index = (z * cols + x) * 3;
            int uv_index = (z * cols + x) * 2;

            // Guardar posición
            vertices[vertex_index + 0] = static_cast<float>(x); // Posición X
            vertices[vertex_index + 1] = y;                    // Altura Y
            vertices[vertex_index + 2] = static_cast<float>(z); // Posición Z

            // Normal inicial (puede calcularse después)
            vertices[vertex_index + 3] = 0.0f;
            vertices[vertex_index + 4] = 1.0f;
            vertices[vertex_index + 5] = 0.0f;

            // Coordenadas UV
            vertices[vertex_index + 6] = static_cast<float>(x) / (cols - 1);
            vertices[vertex_index + 7] = static_cast<float>(z) / (rows - 1);
        }
    }

    stbi_image_free(data);
}

void Heightmap::generate_mesh(float width, float depth) {
    float x_scale = width / (cols - 1);
    float z_scale = depth / (rows - 1);

    indices.reserve((rows - 1) * (cols - 1) * 6); // Cada celda tiene 2 triángulos, cada triángulo 3 índices

    for (int z = 0; z < rows; ++z) {
        for (int x = 0; x < cols; ++x) {
            // Escalar los vértices
            int vertex_index = (z * cols + x) * 8;
            vertices[vertex_index + 0] *= x_scale;
            vertices[vertex_index + 2] *= z_scale;

            // Crear índices para los triángulos
            if (x < cols - 1 && z < rows - 1) {
                int top_left = z * cols + x;
                int top_right = top_left + 1;
                int bottom_left = (z + 1) * cols + x;
                int bottom_right = bottom_left + 1;

                indices.push_back(top_left);
                indices.push_back(bottom_left);
                indices.push_back(top_right);

                indices.push_back(top_right);
                indices.push_back(bottom_left);
                indices.push_back(bottom_right);
            }
        }
    }
}

void Heightmap::render() {
    glBindVertexArray(vao_id);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}