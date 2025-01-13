// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Cone.hpp"
#include <numbers>

namespace udit
{

    /**
     * @brief Constructor de la clase Cone.
     *
     * Este constructor genera los vértices, colores, coordenadas UV e índices necesarios para el cono.
     * El cono se divide en segmentos radiales para la base. El radio y la altura se utilizan para calcular
     * la geometría del cono. Se generan tanto los vértices de la base como los de la cara lateral.
     *
     * @param radial_segments Número de segmentos radiales en la base del cono.
     * @param radius El radio de la base del cono.
     * @param height La altura del cono.
     */
    Cone::Cone(int radial_segments, float radius, float height)
    {
        // Generar los vértices de la base del cono:
        for (int i = 0; i < radial_segments; ++i)
        {
            // Calcular el ángulo de cada segmento radiales
            float angle = 2.0f * std::numbers::pi * static_cast<float>(i) / radial_segments;
            float x = radius * cos(angle);  // Coordenada X en la base
            float z = radius * sin(angle);  // Coordenada Z en la base

            // Guardar las coordenadas de los vértices de la base
            coordinates.push_back(x);
            coordinates.push_back(0.0f);  // Y = 0 para la base
            coordinates.push_back(z);

            // Asignar el color blanco a los vértices de la base
            colors.push_back(1.0f);
            colors.push_back(1.0f);
            colors.push_back(1.0f);

            // Asignar coordenadas UV para la base
            float u = 0.5f + 0.5f * cos(2.0f * std::numbers::pi * i / radial_segments);
            float v = 0.5f + 0.5f * sin(2.0f * std::numbers::pi * i / radial_segments);
            uvs.push_back(u);
            uvs.push_back(v);
        }

        // Vértice central de la base:
        coordinates.push_back(0.0f);  // X
        coordinates.push_back(0.0f);  // Y
        coordinates.push_back(0.0f);  // Z

        colors.push_back(1.0f);  // Rojo para el vértice central
        colors.push_back(0.0f);
        colors.push_back(0.0f);

        uvs.push_back(0.5f);  // UV centrada
        uvs.push_back(0.5f);

        int base_center_index = radial_segments;  // Índice para el vértice central

        // Generar el vértice del ápice:
        coordinates.push_back(0.0f);  // X
        coordinates.push_back(height);  // Y (en la altura máxima)
        coordinates.push_back(0.0f);  // Z

        colors.push_back(0.0f);  // Azul para el vértice del ápice
        colors.push_back(0.0f);
        colors.push_back(1.0f);

        uvs.push_back(0.5f);  // UV centrada
        uvs.push_back(0.5f);

        int apex_index = radial_segments + 1;  // Índice para el vértice del ápice

        // Generar índices para la base del cono:
        // El centro de la base se conecta con cada par de vértices de la base para formar los triángulos
        for (int i = 0; i < radial_segments; ++i)
        {
            indices.push_back(base_center_index);  // Vértice central
            indices.push_back(i);  // Vértice de la base
            indices.push_back((i + 1) % radial_segments);  // Vértice siguiente (índice cíclico)
        }

        // Generar índices para las caras laterales del cono:
        // Cada triángulo se forma conectando el vértice del ápice con dos vértices consecutivos de la base
        for (int i = 0; i < radial_segments; ++i)
        {
            indices.push_back(i);  // Vértice de la base
            indices.push_back(apex_index);  // Vértice del ápice
            indices.push_back((i + 1) % radial_segments);  // Vértice siguiente de la base
        }

        // Crear y configurar los buffers de OpenGL:
        glGenBuffers(VBO_COUNT, vbo_ids);  // Crear los buffers (VBOs)
        glGenVertexArrays(1, &vao_id);     // Crear el VAO (Vertex Array Object)

        glBindVertexArray(vao_id);  // Vincular el VAO

        // Coordenadas de los vértices:
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Colores:
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Coordenadas UV:
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[UVS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Índices de los triángulos:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);  // Desvincular el VAO
    }

    /**
     * @brief Destructor de la clase Cone.
     *
     * Elimina los buffers de OpenGL utilizados por el cono (VAO, VBOs, EBO).
     */
    Cone::~Cone()
    {
        glDeleteVertexArrays(1, &vao_id);  // Eliminar el VAO
        glDeleteBuffers(VBO_COUNT, vbo_ids);  // Eliminar los VBOs
    }

    /**
     * @brief Renderiza el cono en la escena.
     *
     * Este método activa el VAO y dibuja los triángulos que componen la malla del cono
     * usando los índices almacenados en el EBO. El modo de polígonos se establece en
     * GL_FILL para dibujar los triángulos con relleno.
     */
    void Cone::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Dibujar con relleno
        glEnable(GL_CULL_FACE);  // Activar el culling para optimizar el renderizado
        glBindVertexArray(vao_id);  // Vincular el VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);  // Dibujar los triángulos usando los índices
        glBindVertexArray(0);  // Desvincular el VAO
    }

}
