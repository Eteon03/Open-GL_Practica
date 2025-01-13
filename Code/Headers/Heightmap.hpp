// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir la inclusi�n m�ltiple del archivo de encabezado

#include <vector>        // Biblioteca para usar el contenedor din�mico std::vector
#include <string>        // Biblioteca para trabajar con cadenas de texto
#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <glm.hpp>       // Biblioteca para operaciones con vectores y matrices en 3D
#include "../Headers/stb_image.h"  // Librer�a para cargar im�genes (usada para leer el heightmap)

class Heightmap {
public:
    /**
     * @brief Constructor de la clase Heightmap.
     *
     * Este constructor carga el heightmap desde una imagen, genera la malla del terreno
     * y configura los buffers de OpenGL para su renderizado.
     *
     * @param heightmap_path Ruta del archivo de imagen que contiene el heightmap.
     * @param width Ancho del terreno generado.
     * @param depth Profundidad del terreno generado.
     * @param max_height La altura m�xima para los valores del heightmap.
     */
    Heightmap(const std::string& heightmap_path, float width, float depth, float max_height);

    /**
     * @brief Destructor de la clase Heightmap.
     *
     * Libera los recursos de OpenGL asociados al heightmap, como los buffers y la textura.
     */
    ~Heightmap();

    /**
     * @brief Renderiza el heightmap en la escena.
     *
     * Este m�todo dibuja la malla del terreno utilizando los datos almacenados en los buffers de OpenGL.
     */
    void render();

private:
    GLuint vao_id;       ///< ID del Vertex Array Object (VAO) utilizado para almacenar los buffers de v�rtices.
    GLuint vbo_id;       ///< ID del Vertex Buffer Object (VBO) utilizado para almacenar los v�rtices del terreno.
    GLuint ebo_id;       ///< ID del Element Buffer Object (EBO) utilizado para almacenar los �ndices de los tri�ngulos.
    GLuint texture_id;   ///< ID de la textura del terreno (si se utiliza una).

    // Vectores para almacenar los datos de la malla del terreno:
    std::vector<GLfloat> vertices;  ///< Coordenadas (X, Y, Z) de los v�rtices del terreno.
    std::vector<GLfloat> normals;   ///< Normales asociadas a cada v�rtice, usadas para iluminaci�n.
    std::vector<GLfloat> uvs;       ///< Coordenadas UV para mapear texturas.
    std::vector<GLuint> indices;    ///< �ndices que definen los tri�ngulos del terreno.

    int rows, cols;  ///< Dimensiones del heightmap (filas y columnas de p�xeles).

    /**
     * @brief Carga el heightmap desde un archivo y genera los v�rtices del terreno.
     *
     * Carga una imagen en escala de grises y calcula las posiciones de los v�rtices
     * en funci�n de la intensidad de los p�xeles. Las posiciones de los v�rtices se
     * almacenan en el vector de v�rtices, junto con las normales y las coordenadas UV.
     *
     * @param path Ruta del archivo del heightmap.
     * @param max_height La altura m�xima para los valores del heightmap.
     */
    void load_heightmap(const std::string& path, float max_height);

    /**
     * @brief Genera la malla del terreno a partir de los v�rtices.
     *
     * Este m�todo genera los �ndices de los tri�ngulos necesarios para formar el terreno
     * usando los v�rtices calculados previamente. Los tri�ngulos se definen conectando
     * los v�rtices adyacentes.
     *
     * @param width Ancho del terreno generado.
     * @param depth Profundidad del terreno generado.
     */
    void generate_mesh(float width, float depth);
};

