// Este código es de dominio público
// davidbercialblazquez@gmail.com

// Plane.hpp

#pragma once  // Prevenir la inclusión múltiple del archivo de encabezado

#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <vector>        // Biblioteca para usar el contenedor dinámico std::vector

namespace udit
{

    /**
     * @class Plane
     * @brief Clase que representa un plano 3D en OpenGL.
     *
     * Esta clase genera un plano 3D, calcula sus vértices, colores, coordenadas UV e índices,
     * y utiliza los buffers de OpenGL para su renderizado eficiente. El tamaño del plano se
     * define por el número de segmentos en el ancho y alto del plano.
     */
    class Plane
    {
    private:

        // Índices para indexar el array vbo_ids:
        // Se definen los índices que corresponden a los diferentes VBOs y EBO que se usarán en OpenGL
        enum
        {
            COORDINATES_VBO,  ///< VBO para las coordenadas de los vértices
            COLORS_VBO,       ///< VBO para los colores de los vértices
            UVS_VBO,          ///< VBO para las coordenadas UV
            INDICES_EBO,      ///< EBO para los índices de los triángulos
            VBO_COUNT         ///< Número total de VBOs utilizados
        };

        // Arrays dinámicos para almacenar los datos del plano:
        // Vectores para almacenar las coordenadas de los vértices, colores, índices y coordenadas UV
        std::vector<GLfloat> coordinates;  ///< Coordenadas de los vértices (X, Y, Z)
        std::vector<GLfloat> colors;      ///< Colores de los vértices (RGB)
        std::vector<GLubyte> indices;     ///< Índices que definen los triángulos del plano
        std::vector<GLfloat> uvs;         ///< Coordenadas UV para texturización

        GLuint vbo_ids[VBO_COUNT];        ///< Array que contiene los IDs de los VBOs
        GLuint vao_id;                    ///< ID del VAO (Vertex Array Object) que organiza los VBOs

    public:

        /**
         * @brief Constructor de la clase Plane.
         *
         * Este constructor genera los vértices, colores, coordenadas UV e índices necesarios
         * para crear el plano. El número de segmentos en los ejes X y Y determina la resolución
         * de la malla del plano.
         *
         * @param width El número de segmentos a lo largo del eje X del plano.
         * @param height El número de segmentos a lo largo del eje Y del plano.
         */
        Plane(int width, int height);

        /**
         * @brief Destructor de la clase Plane.
         *
         * Elimina los buffers de OpenGL (VAO, VBOs y EBO) cuando el objeto Plane es destruido.
         */
        ~Plane();

        /**
         * @brief Renderiza el plano en la escena.
         *
         * Este método utiliza los buffers de OpenGL configurados previamente (VAO, VBOs, EBO)
         * para renderizar el plano en la escena.
         */
        void render();

    };

}
