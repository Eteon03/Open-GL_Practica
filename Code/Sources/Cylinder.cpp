// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#include "../Headers/Cylinder.hpp"
#include <numbers>

namespace udit
{

    /**
     * @brief Constructor de la clase Cylinder.
     *
     * Genera los v�rtices, colores, coordenadas UV e �ndices necesarios para renderizar un cilindro en 3D,
     * basado en el n�mero de segmentos radiales y de altura proporcionados. El cilindro se divide en segmentos
     * tanto en la base como en las caras laterales.
     *
     * @param radial_segments N�mero de segmentos radiales para la base del cilindro.
     * @param height_segments N�mero de segmentos en la altura del cilindro.
     * @param radius Radio de la base del cilindro.
     * @param height Altura del cilindro.
     */
    Cylinder::Cylinder(int radial_segments, int height_segments, float radius, float height)
    {
        // Generar los v�rtices del cuerpo del cilindro y sus colores:
        for (int y = 0; y <= height_segments; ++y)
        {
            float y_pos = height * (static_cast<float>(y) / height_segments);  // Posici�n Y de cada v�rtice
            for (int x = 0; x <= radial_segments; ++x)
            {
                float angle = 2.0f * std::numbers::pi * (static_cast<float>(x) / radial_segments);  // �ngulo en la base
                float x_pos = radius * cos(angle);  // Coordenada X de cada v�rtice en funci�n del radio y el �ngulo
                float z_pos = radius * sin(angle);  // Coordenada Z de cada v�rtice en funci�n del radio y el �ngulo

                // Coordenadas de los v�rtices
                coordinates.push_back(x_pos);
                coordinates.push_back(y_pos);
                coordinates.push_back(z_pos);

                // Coordenadas UV para las caras laterales
                float u = static_cast<float>(x) / radial_segments;
                float v = static_cast<float>(y) / height_segments;
                uvs.push_back(u);
                uvs.push_back(v);

                // Colores interpolados (ejemplo: degradado radial)
                colors.push_back(static_cast<GLfloat>(x) / radial_segments);  // Color en funci�n de la posici�n X
                colors.push_back(static_cast<GLfloat>(y) / height_segments);  // Color en funci�n de la posici�n Y
                colors.push_back(1.0f);  // Componente azul fijo
            }
        }

        // A�adir v�rtices centrales para las bases:
        coordinates.push_back(0.0f);  // Centro inferior
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);

        uvs.push_back(0.5f);
        uvs.push_back(0.5f);

        colors.push_back(1.0f);  // Color fijo (rojo) para el centro inferior
        colors.push_back(0.0f);
        colors.push_back(0.0f);

        coordinates.push_back(0.0f);  // Centro superior
        coordinates.push_back(height);
        coordinates.push_back(0.0f);

        uvs.push_back(0.5f);
        uvs.push_back(0.5f);

        colors.push_back(0.0f);  // Color fijo (verde) para el centro superior
        colors.push_back(1.0f);
        colors.push_back(0.0f);

        // Generar los �ndices del cuerpo:
        for (int y = 0; y < height_segments; ++y)
        {
            for (int x = 0; x < radial_segments; ++x)
            {
                int current = y * (radial_segments + 1) + x;  // �ndice del v�rtice actual
                int next = current + radial_segments + 1;  // �ndice del siguiente v�rtice en la siguiente fila

                // Tri�ngulo 1 (caras laterales del cilindro)
                indices.push_back(current);
                indices.push_back(current + 1);
                indices.push_back(next);

                // Tri�ngulo 2 (caras laterales del cilindro)
                indices.push_back(current + 1);
                indices.push_back(next + 1);
                indices.push_back(next);
            }
        }

        // Generar �ndices para las bases:
        int base_center_index = coordinates.size() / 3 - 2; // Centro inferior
        int top_center_index = coordinates.size() / 3 - 1;  // Centro superior

        for (int x = 0; x < radial_segments; ++x)
        {
            // Base inferior
            indices.push_back(base_center_index);
            indices.push_back((x + 1) % radial_segments);  // Asegura que se conecten los v�rtices de la base
            indices.push_back(x);

            // Base superior
            indices.push_back(top_center_index);
            indices.push_back((height_segments * (radial_segments + 1)) + (x + 1) % radial_segments);  // Asegura que se conecten los v�rtices superiores
            indices.push_back((height_segments * (radial_segments + 1)) + x);
        }

        // Crear y configurar los buffers de OpenGL:
        glGenBuffers(VBO_COUNT, vbo_ids);  // Generar los buffers
        glGenVertexArrays(1, &vao_id);     // Generar el VAO (Vertex Array Object)

        glBindVertexArray(vao_id);  // Enlazar el VAO

        // Coordenadas de los v�rtices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Colores
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Coordenadas UV
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[UVS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // �ndices de los tri�ngulos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);  // Desvincular el VAO
    }


    /**
     * @brief Destructor de la clase Cylinder.
     *
     * Libera los recursos de OpenGL asociados al cilindro, eliminando los buffers y el VAO.
     */
    Cylinder::~Cylinder()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    /**
     * @brief Renderiza el cilindro en la escena.
     *
     * Configura el modo de dibujo para el cilindro y usa los �ndices de los v�rtices para dibujar los tri�ngulos
     * que forman las caras del cilindro y sus bases.
     */
    void Cylinder::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Dibuja el cilindro con relleno
        glDisable(GL_CULL_FACE);  // Desactiva el culling (no recorta caras)
        glBindVertexArray(vao_id);  // Vincula el VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);  // Dibuja los tri�ngulos utilizando los �ndices
        glBindVertexArray(0);  // Desvincula el VAO
    }

}
