// Este código es de dominio público
// davidbercialblazquez@gmail.com


#include "../Headers/Cylinder.hpp"
#include <numbers>

namespace udit
{

    Cylinder::Cylinder(int radial_segments, int height_segments, float radius, float height)
    {
        // Generar los vértices del cuerpo del cilindro y sus colores:
        for (int y = 0; y <= height_segments; ++y)
        {
            float y_pos = height * (static_cast<float>(y) / height_segments);
            for (int x = 0; x <= radial_segments; ++x)
            {
                float angle = 2.0f * std::numbers::pi * (static_cast<float>(x) / radial_segments);
                float x_pos = radius * cos(angle);
                float z_pos = radius * sin(angle);

                // Coordenadas de los vértices
                coordinates.push_back(x_pos);
                coordinates.push_back(y_pos);
                coordinates.push_back(z_pos);

                // Colores interpolados (ejemplo: degradado radial)
                colors.push_back(static_cast<GLfloat>(x) / radial_segments);
                colors.push_back(static_cast<GLfloat>(y) / height_segments);
                colors.push_back(1.0f); // Azul fijo
            }
        }

        // Añadir vértices centrales para las bases:
        coordinates.push_back(0.0f); // Centro inferior
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);

        colors.push_back(1.0f); // Color fijo
        colors.push_back(0.0f);
        colors.push_back(0.0f);

        coordinates.push_back(0.0f); // Centro superior
        coordinates.push_back(height);
        coordinates.push_back(0.0f);

        colors.push_back(0.0f); // Color fijo
        colors.push_back(1.0f);
        colors.push_back(0.0f);

        // Generar los índices del cuerpo:
        for (int y = 0; y < height_segments; ++y)
        {
            for (int x = 0; x < radial_segments; ++x)
            {
                int current = y * (radial_segments + 1) + x;
                int next = current + radial_segments + 1;

                // Triángulo 1
                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                // Triángulo 2
                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        // Generar índices para las bases:
        int base_center_index = coordinates.size() / 3 - 2; // Centro inferior
        int top_center_index = coordinates.size() / 3 - 1;  // Centro superior

        for (int x = 0; x < radial_segments; ++x)
        {
            // Base inferior
            indices.push_back(base_center_index);
            indices.push_back(x);
            indices.push_back((x + 1) % radial_segments);

            // Base superior
            indices.push_back(top_center_index);
            indices.push_back((height_segments * (radial_segments + 1)) + x);
            indices.push_back((height_segments * (radial_segments + 1)) + (x + 1) % radial_segments);
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


    Cylinder::~Cylinder()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cylinder::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }

}
