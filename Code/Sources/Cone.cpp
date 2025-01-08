// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Cone.hpp"
#include <numbers>

namespace udit
{

    Cone::Cone(int radial_segments, float radius, float height)
    {
        // Generar los vértices de la base:
        for (int i = 0; i < radial_segments; ++i)
        {
            float angle = 2.0f * std::numbers::pi * static_cast<float>(i) / radial_segments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Coordenadas de la base
            coordinates.push_back(x);
            coordinates.push_back(0.0f);
            coordinates.push_back(z);

            // Colores de la base (ejemplo: blanco)
            colors.push_back(1.0f);
            colors.push_back(1.0f);
            colors.push_back(1.0f);


            float u = 0.5f + 0.5f * cos(2.0f * std::numbers::pi * i / radial_segments);
            float v = 0.5f + 0.5f * sin(2.0f * std::numbers::pi * i / radial_segments);

            // Coordenadas UV de la base
            uvs.push_back(u);
            uvs.push_back(v);
        }

        // Vértice central de la base
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);

        colors.push_back(1.0f);
        colors.push_back(0.0f);
        colors.push_back(0.0f);

        uvs.push_back(0.5f);
        uvs.push_back(0.5f);

        int base_center_index = radial_segments;

        // Generar el vértice del ápice:
        coordinates.push_back(0.0f);
        coordinates.push_back(height);
        coordinates.push_back(0.0f);

        colors.push_back(0.0f);
        colors.push_back(0.0f);
        colors.push_back(1.0f);

        uvs.push_back(0.5f);
        uvs.push_back(0.5f);

        int apex_index = radial_segments + 1;

        // Generar índices para la base:
        for (int i = 0; i < radial_segments; ++i)
        {
            indices.push_back(base_center_index);
            indices.push_back(i);
            indices.push_back((i + 1) % radial_segments);
        }

        // Generar índices para las caras laterales:
        for (int i = 0; i < radial_segments; ++i)
        {
            indices.push_back(i);
            indices.push_back(apex_index);
            indices.push_back((i + 1) % radial_segments);
            
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

        // UVs
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[UVS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Cone::~Cone()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cone::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }

}