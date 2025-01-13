// Este código es de dominio público
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir la inclusión múltiple del archivo de encabezado

#include <string>        // Biblioteca para trabajar con cadenas de texto
#include <vector>        // Biblioteca para usar el contenedor dinámico std::vector
#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <glm.hpp>       // Biblioteca para operaciones con vectores y matrices en 3D
#include "../Headers/stb_image.h"  // Librería para cargar imágenes (utilizada para las texturas)

class Skybox {
public:
    /**
     * @brief Constructor de la clase Skybox.
     *
     * Este constructor recibe un vector de rutas de imágenes que representan las seis caras
     * de la skybox, y carga las texturas correspondientes para el cubo que forma el fondo.
     *
     * @param faces Rutas de las imágenes para las seis caras de la skybox (en orden: derecha, izquierda, arriba, abajo, adelante, atrás).
     */
    Skybox(const std::vector<std::string>& faces);

    /**
     * @brief Destructor de la clase Skybox.
     *
     * Libera los recursos de OpenGL asociados a la skybox, como los buffers y la textura.
     */
    ~Skybox();

    /**
     * @brief Establece la textura para la skybox.
     *
     * Asocia una textura específica a la skybox.
     *
     * @param texture_id El ID de la textura que se va a asignar a la skybox.
     */
    void set_texture(GLuint texture_id);

    /**
     * @brief Obtiene el ID de la textura de la skybox.
     *
     * Retorna el ID de la textura que se está utilizando para la skybox.
     *
     * @return GLuint El ID de la textura.
     */
    GLuint get_texture_id();

    /**
     * @brief Renderiza la skybox en la escena.
     *
     * Este método dibuja el cubo de la skybox en el fondo de la escena.
     */
    void render();

private:
    GLuint vao_id;         ///< ID del Vertex Array Object (VAO) utilizado para la skybox.
    GLuint vbo_id;         ///< ID del Vertex Buffer Object (VBO) utilizado para almacenar los vértices de la skybox.
    GLuint texture_id;     ///< ID de la textura de la skybox.
    std::vector<std::string> faces;  ///< Rutas de las imágenes de las seis caras de la skybox.

    /**
     * @brief Carga las texturas de las seis caras de la skybox.
     *
     * Este método carga las imágenes desde las rutas proporcionadas y crea una textura de tipo cubemap.
     */
    void load_textures();

    /**
     * @brief Configura los buffers de OpenGL para la skybox.
     *
     * Este método crea los buffers necesarios (VAO, VBO) y organiza los datos de los vértices para la skybox.
     */
    void setup_buffers();
};
