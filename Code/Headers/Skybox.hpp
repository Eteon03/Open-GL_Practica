// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir la inclusi�n m�ltiple del archivo de encabezado

#include <string>        // Biblioteca para trabajar con cadenas de texto
#include <vector>        // Biblioteca para usar el contenedor din�mico std::vector
#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include <glm.hpp>       // Biblioteca para operaciones con vectores y matrices en 3D
#include "../Headers/stb_image.h"  // Librer�a para cargar im�genes (utilizada para las texturas)

class Skybox {
public:
    /**
     * @brief Constructor de la clase Skybox.
     *
     * Este constructor recibe un vector de rutas de im�genes que representan las seis caras
     * de la skybox, y carga las texturas correspondientes para el cubo que forma el fondo.
     *
     * @param faces Rutas de las im�genes para las seis caras de la skybox (en orden: derecha, izquierda, arriba, abajo, adelante, atr�s).
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
     * Asocia una textura espec�fica a la skybox.
     *
     * @param texture_id El ID de la textura que se va a asignar a la skybox.
     */
    void set_texture(GLuint texture_id);

    /**
     * @brief Obtiene el ID de la textura de la skybox.
     *
     * Retorna el ID de la textura que se est� utilizando para la skybox.
     *
     * @return GLuint El ID de la textura.
     */
    GLuint get_texture_id();

    /**
     * @brief Renderiza la skybox en la escena.
     *
     * Este m�todo dibuja el cubo de la skybox en el fondo de la escena.
     */
    void render();

private:
    GLuint vao_id;         ///< ID del Vertex Array Object (VAO) utilizado para la skybox.
    GLuint vbo_id;         ///< ID del Vertex Buffer Object (VBO) utilizado para almacenar los v�rtices de la skybox.
    GLuint texture_id;     ///< ID de la textura de la skybox.
    std::vector<std::string> faces;  ///< Rutas de las im�genes de las seis caras de la skybox.

    /**
     * @brief Carga las texturas de las seis caras de la skybox.
     *
     * Este m�todo carga las im�genes desde las rutas proporcionadas y crea una textura de tipo cubemap.
     */
    void load_textures();

    /**
     * @brief Configura los buffers de OpenGL para la skybox.
     *
     * Este m�todo crea los buffers necesarios (VAO, VBO) y organiza los datos de los v�rtices para la skybox.
     */
    void setup_buffers();
};
