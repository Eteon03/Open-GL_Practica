// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir inclusi�n m�ltiple del archivo de encabezado

#include <glad/glad.h>  // Biblioteca para cargar funciones de OpenGL
#include <vector>        // Biblioteca para usar el contenedor din�mico std::vector

namespace udit
{

    /**
     * @class Cylinder
     * @brief Clase que representa un cilindro 3D en OpenGL.
     *
     * Esta clase genera los v�rtices, colores, coordenadas UV e �ndices necesarios para renderizar
     * un cilindro con un n�mero especificado de segmentos radiales y de altura. Tambi�n configura los
     * buffers de OpenGL para almacenar y organizar estos datos de manera eficiente.
     */
    class Cylinder
    {
    private:

        // �ndices para indexar el array vbo_ids:
        // Los VBOs almacenan los datos de las coordenadas, colores, UVs y los �ndices
        // El EBO (Element Buffer Object) almacena los �ndices que forman los tri�ngulos de la malla.
        enum
        {
            COORDINATES_VBO,  ///< VBO que almacena las coordenadas de los v�rtices
            COLORS_VBO,       ///< VBO que almacena los colores de los v�rtices
            UVS_VBO,          ///< VBO que almacena las coordenadas de textura (UVs)
            INDICES_EBO,      ///< EBO que almacena los �ndices para los tri�ngulos
            VBO_COUNT         ///< Total de VBOs utilizados
        };

        // Arrays din�micos para almacenar los datos del cilindro:
        // Se usan vectores din�micos para almacenar los datos, que luego ser�n enviados a los buffers de OpenGL
        std::vector<GLfloat> coordinates;  ///< V�rtices del cilindro
        std::vector<GLfloat> colors;      ///< Colores de los v�rtices
        std::vector<GLubyte> indices;     ///< �ndices que definen las caras del cilindro
        std::vector<GLfloat> uvs;         ///< Coordenadas UV para las texturas

        GLuint vbo_ids[VBO_COUNT];        ///< Ids de los VBOs que se usan
        GLuint vao_id;                    ///< Id del VAO (Vertex Array Object) del cilindro

    public:

        /**
         * @brief Constructor de la clase Cylinder.
         *
         * Este constructor genera los v�rtices, colores, coordenadas UV e �ndices necesarios
         * para renderizar el cilindro. Los segmentos radiales y de altura permiten definir
         * la resoluci�n del cilindro. El radio y la altura definen las dimensiones del cilindro.
         *
         * @param radial_segments N�mero de segmentos radiales de la base del cilindro.
         * @param height_segments N�mero de segmentos de altura del cilindro.
         * @param radius Radio de la base del cilindro.
         * @param height Altura del cilindro.
         */
        Cylinder(int radial_segments, int height_segments, float radius, float height);

        /**
         * @brief Destructor de la clase Cylinder.
         *
         * Elimina los recursos de OpenGL utilizados por el cilindro, como los VBOs y el VAO.
         */
        ~Cylinder();

        /**
         * @brief Renderiza el cilindro en la escena.
         *
         * Este m�todo configura los atributos de OpenGL necesarios y dibuja el cilindro usando los
         * datos almacenados en los buffers (VBOs y EBO).
         */
        void render();
    };

}
