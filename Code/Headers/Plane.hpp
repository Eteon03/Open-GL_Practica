// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

// Plane.hpp

#pragma once  // Prevenir la inclusi�n m�ltiple del archivo de encabezado

#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <vector>        // Biblioteca para usar el contenedor din�mico std::vector

namespace udit
{

    /**
     * @class Plane
     * @brief Clase que representa un plano 3D en OpenGL.
     *
     * Esta clase genera un plano 3D, calcula sus v�rtices, colores, coordenadas UV e �ndices,
     * y utiliza los buffers de OpenGL para su renderizado eficiente. El tama�o del plano se
     * define por el n�mero de segmentos en el ancho y alto del plano.
     */
    class Plane
    {
    private:

        // �ndices para indexar el array vbo_ids:
        // Se definen los �ndices que corresponden a los diferentes VBOs y EBO que se usar�n en OpenGL
        enum
        {
            COORDINATES_VBO,  ///< VBO para las coordenadas de los v�rtices
            COLORS_VBO,       ///< VBO para los colores de los v�rtices
            UVS_VBO,          ///< VBO para las coordenadas UV
            INDICES_EBO,      ///< EBO para los �ndices de los tri�ngulos
            VBO_COUNT         ///< N�mero total de VBOs utilizados
        };

        // Arrays din�micos para almacenar los datos del plano:
        // Vectores para almacenar las coordenadas de los v�rtices, colores, �ndices y coordenadas UV
        std::vector<GLfloat> coordinates;  ///< Coordenadas de los v�rtices (X, Y, Z)
        std::vector<GLfloat> colors;      ///< Colores de los v�rtices (RGB)
        std::vector<GLubyte> indices;     ///< �ndices que definen los tri�ngulos del plano
        std::vector<GLfloat> uvs;         ///< Coordenadas UV para texturizaci�n

        GLuint vbo_ids[VBO_COUNT];        ///< Array que contiene los IDs de los VBOs
        GLuint vao_id;                    ///< ID del VAO (Vertex Array Object) que organiza los VBOs

    public:

        /**
         * @brief Constructor de la clase Plane.
         *
         * Este constructor genera los v�rtices, colores, coordenadas UV e �ndices necesarios
         * para crear el plano. El n�mero de segmentos en los ejes X y Y determina la resoluci�n
         * de la malla del plano.
         *
         * @param width El n�mero de segmentos a lo largo del eje X del plano.
         * @param height El n�mero de segmentos a lo largo del eje Y del plano.
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
         * Este m�todo utiliza los buffers de OpenGL configurados previamente (VAO, VBOs, EBO)
         * para renderizar el plano en la escena.
         */
        void render();

    };

}
