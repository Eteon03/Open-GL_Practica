// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir la inclusi�n m�ltiple del archivo de encabezado

#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <vector>        // Biblioteca para usar el contenedor din�mico std::vector

namespace udit
{

    /**
     * @class Cone
     * @brief Clase que representa un cono 3D en OpenGL.
     *
     * Esta clase genera un cono 3D mediante la creaci�n de v�rtices, colores, coordenadas UV
     * e �ndices para la malla. Adem�s, gestiona los buffers de OpenGL (VAO, VBO, EBO) necesarios
     * para su renderizaci�n.
     */
    class Cone
    {
    private:

        // �ndices para indexar el array vbo_ids:
        // Se definen los �ndices para los buffers de OpenGL que se usar�n:
        // - COORDINATES_VBO: Para los v�rtices
        // - COLORS_VBO: Para los colores
        // - UVS_VBO: Para las coordenadas UV
        // - INDICES_EBO: Para los �ndices que definen los tri�ngulos de la malla
        enum
        {
            COORDINATES_VBO,  ///< VBO que almacena las coordenadas de los v�rtices
            COLORS_VBO,       ///< VBO que almacena los colores de los v�rtices
            UVS_VBO,          ///< VBO que almacena las coordenadas UV de los v�rtices
            INDICES_EBO,      ///< EBO que almacena los �ndices para los tri�ngulos de la malla
            VBO_COUNT         ///< Cantidad de VBOs utilizados
        };

        // Arrays din�micos para almacenar los datos del cono:
        // Se utilizan vectores din�micos para almacenar las coordenadas, colores, �ndices y coordenadas UV
        std::vector<GLfloat> coordinates;  ///< Coordenadas (X, Y, Z) de los v�rtices
        std::vector<GLfloat> colors;      ///< Colores de cada v�rtice
        std::vector<GLubyte> indices;     ///< �ndices que definen los tri�ngulos del cono
        std::vector<GLfloat> uvs;         ///< Coordenadas UV para mapear texturas

        GLuint vbo_ids[VBO_COUNT];        ///< Array que guarda los IDs de los VBOs
        GLuint vao_id;                    ///< ID del VAO (Vertex Array Object) que organiza los VBOs

    public:

        /**
         * @brief Constructor de la clase Cone.
         *
         * Genera los v�rtices, colores, coordenadas UV e �ndices necesarios para el cono. Los
         * segmentos radiales definen cu�ntos segmentos tiene la base del cono, mientras que el radio
         * y la altura definen las dimensiones del cono.
         *
         * @param radial_segments N�mero de segmentos radiales en la base del cono.
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
         * Este m�todo usa los buffers de OpenGL configurados para dibujar el cono en la escena.
         */
        void render();

    };

}
