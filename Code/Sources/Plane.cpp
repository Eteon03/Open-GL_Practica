// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#include "../Headers/Plane.hpp"

namespace udit
{

    /**
     * @brief Constructor de la clase Plane.
     *
     * Este constructor genera los v�rtices, colores, coordenadas UV e �ndices necesarios
     * para representar un plano 3D. El plano se divide en segmentos, cuyo n�mero se determina
     * por los par�metros `width` y `height`. El plano se crea en el plano XZ con altura fija en 0.
     * Los colores se interpolan para crear un degradado a trav�s de la malla del plano.
     *
     * @param width El n�mero de segmentos en el eje X (ancho) del plano.
     * @param height El n�mero de segmentos en el eje Y (alto) del plano.
     */
    Plane::Plane(int width, int height)
    {
        // Generar los v�rtices del plano y sus colores:
        for (int y = 0; y <= height; ++y)  // Itera a lo largo del eje Y
        {
            for (int x = 0; x <= width; ++x)  // Itera a lo largo del eje X
            {
                // Coordenadas de los v�rtices (en el plano XZ, con Y = 0)
                coordinates.push_back(x);
                coordinates.push_back(0); // Altura fija en 0 (plano en XZ)
                coordinates.push_back(y);

                // Colores de los v�rtices, interpolados en funci�n de su posici�n en el plano
                colors.push_back(static_cast<GLfloat>(x) / width);  // Color en el eje X
                colors.push_back(static_cast<GLfloat>(y) / height); // Color en el eje Y
                colors.push_back(1.0f);  // Componente azul fijo (ejemplo de color)

                // Coordenadas UV, interpoladas para mapear una textura
                uvs.push_back(static_cast<GLfloat>(x) / width);
                uvs.push_back(static_cast<GLfloat>(y) / height);
            }
        }

        // Generar los �ndices para formar los tri�ngulos del plano:
        for (int y = 0; y < height; ++y)  // Iterar sobre las filas del plano
        {
            for (int x = 0; x < width; ++x)  // Iterar sobre las columnas del plano
            {
                // Calcular los �ndices de los v�rtices de cada celda del plano
                int topLeft = y * (width + 1) + x;  // �ndice del v�rtice superior izquierdo
                int topRight = topLeft + 1;  // �ndice del v�rtice superior derecho
                int bottomLeft = topLeft + (width + 1);  // �ndice del v�rtice inferior izquierdo
                int bottomRight = bottomLeft + 1;  // �ndice del v�rtice inferior derecho

                // Tri�ngulo 1 (parte inferior izquierda)
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Tri�ngulo 2 (parte superior derecha)
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        // Crear y configurar los buffers de OpenGL:
        glGenBuffers(VBO_COUNT, vbo_ids);  // Crear los VBOs
        glGenVertexArrays(1, &vao_id);     // Crear el VAO (Vertex Array Object)

        glBindVertexArray(vao_id);  // Vincular el VAO

        // Coordenadas de los v�rtices (VBO)
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

        // �ndices (EBO)
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
     * Este m�todo utiliza los buffers de OpenGL (VAO, VBOs, EBO) para dibujar el plano en la escena.
     * Los �ndices de los tri�ngulos son usados para renderizar la malla del plano.
     */
    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Dibujar el plano con relleno (no contornos)
        glDisable(GL_CULL_FACE);  // Desactivar el culling para que el plano sea visible desde ambos lados

        glBindVertexArray(vao_id);  // Vincular el VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);  // Dibujar los tri�ngulos usando los �ndices
        glBindVertexArray(0);  // Desvincular el VAO
    }

}
