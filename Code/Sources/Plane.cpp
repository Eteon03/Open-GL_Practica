// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Plane.hpp"

namespace udit
{

    /**
     * @brief Constructor de la clase Plane.
     *
     * Este constructor genera los vértices, colores, coordenadas UV e índices necesarios
     * para representar un plano 3D. El plano se divide en segmentos, cuyo número se determina
     * por los parámetros `width` y `height`. El plano se crea en el plano XZ con altura fija en 0.
     * Los colores se interpolan para crear un degradado a través de la malla del plano.
     *
     * @param width El número de segmentos en el eje X (ancho) del plano.
     * @param height El número de segmentos en el eje Y (alto) del plano.
     */
    Plane::Plane(int width, int height)
    {
        // Generar los vértices del plano y sus colores:
        for (int y = 0; y <= height; ++y)  // Itera a lo largo del eje Y
        {
            for (int x = 0; x <= width; ++x)  // Itera a lo largo del eje X
            {
                // Coordenadas de los vértices (en el plano XZ, con Y = 0)
                coordinates.push_back(x);
                coordinates.push_back(0); // Altura fija en 0 (plano en XZ)
                coordinates.push_back(y);

                // Colores de los vértices, interpolados en función de su posición en el plano
                colors.push_back(static_cast<GLfloat>(x) / width);  // Color en el eje X
                colors.push_back(static_cast<GLfloat>(y) / height); // Color en el eje Y
                colors.push_back(1.0f);  // Componente azul fijo (ejemplo de color)

                // Coordenadas UV, interpoladas para mapear una textura
                uvs.push_back(static_cast<GLfloat>(x) / width);
                uvs.push_back(static_cast<GLfloat>(y) / height);
            }
        }

        // Generar los índices para formar los triángulos del plano:
        for (int y = 0; y < height; ++y)  // Iterar sobre las filas del plano
        {
            for (int x = 0; x < width; ++x)  // Iterar sobre las columnas del plano
            {
                // Calcular los índices de los vértices de cada celda del plano
                int topLeft = y * (width + 1) + x;  // Índice del vértice superior izquierdo
                int topRight = topLeft + 1;  // Índice del vértice superior derecho
                int bottomLeft = topLeft + (width + 1);  // Índice del vértice inferior izquierdo
                int bottomRight = bottomLeft + 1;  // Índice del vértice inferior derecho

                // Triángulo 1 (parte inferior izquierda)
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Triángulo 2 (parte superior derecha)
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        // Crear y configurar los buffers de OpenGL:
        glGenBuffers(VBO_COUNT, vbo_ids);  // Crear los VBOs
        glGenVertexArrays(1, &vao_id);     // Crear el VAO (Vertex Array Object)

        glBindVertexArray(vao_id);  // Vincular el VAO

        // Coordenadas de los vértices (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);  // Activar el atributo para las coordenadas
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // Establecer el puntero a las coordenadas

        // Colores (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);  // Activar el atributo para los colores
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);  // Establecer el puntero a los colores

        // Coordenadas UV (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[UVS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);  // Activar el atributo para las coordenadas UV
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);  // Establecer el puntero a las coordenadas UV

        // Índices (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);  // Desvincular el VAO
    }

    /**
     * @brief Destructor de la clase Plane.
     *
     * Elimina los buffers de OpenGL (VAO, VBOs, EBO) cuando el objeto Plane es destruido.
     */
    Plane::~Plane()
    {
        glDeleteVertexArrays(1, &vao_id);  // Eliminar el VAO
        glDeleteBuffers(VBO_COUNT, vbo_ids);  // Eliminar los VBOs y EBO
    }

    /**
     * @brief Renderiza el plano en la escena.
     *
     * Este método utiliza los buffers de OpenGL (VAO, VBOs, EBO) para dibujar el plano en la escena.
     * Los índices de los triángulos son usados para renderizar la malla del plano.
     */
    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Dibujar el plano con relleno (no contornos)
        glDisable(GL_CULL_FACE);  // Desactivar el culling para que el plano sea visible desde ambos lados

        glBindVertexArray(vao_id);  // Vincular el VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);  // Dibujar los triángulos usando los índices
        glBindVertexArray(0);  // Desvincular el VAO
    }

}
