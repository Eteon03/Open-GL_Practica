// Este código es de dominio público
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir inclusión múltiple del archivo de encabezado

#include <glad/glad.h>  // Biblioteca para cargar funciones de OpenGL
#include <vector>        // Biblioteca para usar el contenedor dinámico std::vector

namespace udit
{

    /**
     * @class Cylinder
     * @brief Clase que representa un cilindro 3D en OpenGL.
     *
     * Esta clase genera los vértices, colores, coordenadas UV e índices necesarios para renderizar
     * un cilindro con un número especificado de segmentos radiales y de altura. También configura los
     * buffers de OpenGL para almacenar y organizar estos datos de manera eficiente.
     */
    class Cylinder
    {
    private:

        // Índices para indexar el array vbo_ids:
        // Los VBOs almacenan los datos de las coordenadas, colores, UVs y los índices
        // El EBO (Element Buffer Object) almacena los índices que forman los triángulos de la malla.
        enum
        {
            COORDINATES_VBO,  ///< VBO que almacena las coordenadas de los vértices
            COLORS_VBO,       ///< VBO que almacena los colores de los vértices
            UVS_VBO,          ///< VBO que almacena las coordenadas de textura (UVs)
            INDICES_EBO,      ///< EBO que almacena los índices para los triángulos
            VBO_COUNT         ///< Total de VBOs utilizados
        };

        // Arrays dinámicos para almacenar los datos del cilindro:
        // Se usan vectores dinámicos para almacenar los datos, que luego serán enviados a los buffers de OpenGL
        std::vector<GLfloat> coordinates;  ///< Vértices del cilindro
        std::vector<GLfloat> colors;      ///< Colores de los vértices
        std::vector<GLubyte> indices;     ///< Índices que definen las caras del cilindro
        std::vector<GLfloat> uvs;         ///< Coordenadas UV para las texturas

        GLuint vbo_ids[VBO_COUNT];        ///< Ids de los VBOs que se usan
        GLuint vao_id;                    ///< Id del VAO (Vertex Array Object) del cilindro

    public:

        /**
         * @brief Constructor de la clase Cylinder.
         *
         * Este constructor genera los vértices, colores, coordenadas UV e índices necesarios
         * para renderizar el cilindro. Los segmentos radiales y de altura permiten definir
         * la resolución del cilindro. El radio y la altura definen las dimensiones del cilindro.
         *
         * @param radial_segments Número de segmentos radiales de la base del cilindro.
         * @param height_segments Número de segmentos de altura del cilindro.
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
         * Este método configura los atributos de OpenGL necesarios y dibuja el cilindro usando los
         * datos almacenados en los buffers (VBOs y EBO).
         */
        void render();
    };

}
