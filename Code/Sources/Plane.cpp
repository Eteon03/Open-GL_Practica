// Este código es de dominio público
// davidbercialblazquez@gmail.com


#include "../Headers/Plane.hpp"

namespace udit
{

    Plane::Plane(int width, int height)
    {
        // Generar los vértices del plano y sus colores:
        for (int y = 0; y <= height; ++y)
        {
            for (int x = 0; x <= width; ++x)
            {
                // Coordenadas de los vértices
                coordinates.push_back(x);
                coordinates.push_back(0); // Altura fija en 0 (plano en XZ)
                coordinates.push_back(y);

                // Colores interpolados (ejemplo: degradado)
                colors.push_back(static_cast<GLfloat>(x) / width);
                colors.push_back(static_cast<GLfloat>(y) / height);
                colors.push_back(1.0f); // Color fijo para el azul
            }
        }

        // Generar los índices del plano:
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int topLeft = y * (width + 1) + x;
                int topRight = topLeft + 1;
                int bottomLeft = topLeft + (width + 1);
                int bottomRight = bottomLeft + 1;

                // Triángulo 1
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Triángulo 2
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        // Crear y configurar los buffers de OpenGL:
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // Coordenadas
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Plane::~Plane()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }

}
