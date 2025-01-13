// Este código es de dominio público
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir la inclusión múltiple del archivo de encabezado

#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <vector>        // Biblioteca para usar el contenedor dinámico std::vector

namespace udit
{

    /**
     * @class Cone
     * @brief Clase que representa un cono 3D en OpenGL.
     *
     * Esta clase genera un cono 3D mediante la creación de vértices, colores, coordenadas UV
     * e índices para la malla. Además, gestiona los buffers de OpenGL (VAO, VBO, EBO) necesarios
     * para su renderización.
     */
    class Cone
    {
    private:

        // Índices para indexar el array vbo_ids:
        // Se definen los índices para los buffers de OpenGL que se usarán:
        // - COORDINATES_VBO: Para los vértices
        // - COLORS_VBO: Para los colores
        // - UVS_VBO: Para las coordenadas UV
        // - INDICES_EBO: Para los índices que definen los triángulos de la malla
        enum
        {
            COORDINATES_VBO,  ///< VBO que almacena las coordenadas de los vértices
            COLORS_VBO,       ///< VBO que almacena los colores de los vértices
            UVS_VBO,          ///< VBO que almacena las coordenadas UV de los vértices
            INDICES_EBO,      ///< EBO que almacena los índices para los triángulos de la malla
            VBO_COUNT         ///< Cantidad de VBOs utilizados
        };

        // Arrays dinámicos para almacenar los datos del cono:
        // Se utilizan vectores dinámicos para almacenar las coordenadas, colores, índices y coordenadas UV
        std::vector<GLfloat> coordinates;  ///< Coordenadas (X, Y, Z) de los vértices
        std::vector<GLfloat> colors;      ///< Colores de cada vértice
        std::vector<GLubyte> indices;     ///< Índices que definen los triángulos del cono
        std::vector<GLfloat> uvs;         ///< Coordenadas UV para mapear texturas

        GLuint vbo_ids[VBO_COUNT];        ///< Array que guarda los IDs de los VBOs
        GLuint vao_id;                    ///< ID del VAO (Vertex Array Object) que organiza los VBOs

    public:

        /**
         * @brief Constructor de la clase Cone.
         *
         * Genera los vértices, colores, coordenadas UV e índices necesarios para el cono. Los
         * segmentos radiales definen cuántos segmentos tiene la base del cono, mientras que el radio
         * y la altura definen las dimensiones del cono.
         *
         * @param radial_segments Número de segmentos radiales en la base del cono.
         * @param radius El radio de la base del cono.
         * @param height La altura del cono.
         */
        Cone(int radial_segments, float radius, float height);

        /**
         * @brief Destructor de la clase Cone.
         *
         * Elimina los recursos de OpenGL utilizados por el cono, como los buffers (VAO, VBO y EBO).
         */
        ~Cone();

        /**
         * @brief Renderiza el cono en la escena.
         *
         * Este método usa los buffers de OpenGL configurados para dibujar el cono en la escena.
         */
        void render();

    };

}
