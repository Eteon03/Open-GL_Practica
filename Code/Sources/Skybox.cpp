// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Skybox.hpp"  // Incluir el encabezado de la clase Skybox
#include <iostream>  // Incluir para poder imprimir mensajes de error y éxito

/**
 * @brief Constructor de la clase Skybox.
 *
 * Inicializa la skybox cargando las texturas para las seis caras del cubo
 * y configurando los buffers necesarios para renderizar el cubo.
 *
 * @param faces Rutas de las imágenes de las seis caras de la skybox (en orden: derecha, izquierda, arriba, abajo, adelante, atrás).
 */
Skybox::Skybox(const std::vector<std::string>& faces)
    : faces(faces) {  // Inicializar la lista de rutas de las caras
    load_textures();  // Cargar las texturas de las seis caras de la skybox
    setup_buffers();  // Configurar los buffers de OpenGL (VAO, VBO)
}

/**
 * @brief Destructor de la clase Skybox.
 *
 * Libera los recursos de OpenGL utilizados por la skybox, incluyendo
 * los buffers (VAO, VBO) y la textura.
 */
Skybox::~Skybox() {
    glDeleteVertexArrays(1, &vao_id);  // Eliminar el VAO
    glDeleteBuffers(1, &vbo_id);       // Eliminar el VBO
    glDeleteTextures(1, &texture_id);  // Eliminar la textura del cubo
}

/**
 * @brief Carga las texturas de las seis caras de la skybox.
 *
 * Este método carga las imágenes de las seis caras de la skybox utilizando
 * la librería stb_image y las asocia a una textura cubemap en OpenGL.
 */
void Skybox::load_textures() {
    glGenTextures(1, &texture_id);  // Crear una textura de OpenGL
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);  // Vincular la textura como un cubemap

    int width, height, channels;
    for (GLuint i = 0; i < faces.size(); ++i) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (!data) {  // Verificar si la imagen se cargó correctamente
            std::cerr << "Error: No se pudo cargar la textura del skybox en " << faces[i] << std::endl;
            std::cerr << "Ruta: " << faces[i] << std::endl;
            continue;
        }
        else {
            std::cout << "Textura cargada correctamente: " << faces[i] << " ("
                << width << "x" << height << ", canales: " << channels << ")" << std::endl;
        }

        // Asignar la textura cargada al cubemap
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);  // Liberar la memoria de la imagen cargada
    }

    // Configurar los parámetros de la textura cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

/**
 * @brief Configura los buffers de OpenGL para la skybox.
 *
 * Este método configura el VAO y el VBO con los vértices de las seis caras del cubo,
 * que formarán la skybox. El cubo tiene 36 vértices (6 caras x 2 triángulos x 3 vértices por triángulo).
 */
void Skybox::setup_buffers() {
    GLfloat vertices[] = {
        // Vértices para las 6 caras del cubo
        // (Cada cara tiene 2 triángulos, cada triángulo tiene 3 vértices)
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // Generar los buffers de OpenGL (VAO y VBO)
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);

    glBindVertexArray(vao_id);  // Vincular el VAO

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);  // Vincular el VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Subir los vértices al VBO

    // Configurar el atributo para las posiciones de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  // Activar el atributo de posición

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Desvincular el VBO
    glBindVertexArray(0);  // Desvincular el VAO
}

/**
 * @brief Establece la textura para la skybox.
 *
 * Este método permite asignar un ID de textura a la skybox, que se usará durante el renderizado.
 *
 * @param texture_id El ID de la textura que se asignará a la skybox.
 */
void Skybox::set_texture(GLuint texture_id) {
    this->texture_id = texture_id;  // Asignar la textura a la skybox
}

/**
 * @brief Obtiene el ID de la textura de la skybox.
 *
 * Este método devuelve el ID de la textura de la skybox, útil si es necesario acceder a él
 * fuera de la clase Skybox.
 *
 * @return GLuint El ID de la textura de la skybox.
 */
GLuint Skybox::get_texture_id()
{
    return texture_id;  // Retornar el ID de la textura
}

/**
 * @brief Renderiza la skybox en la escena.
 *
 * Este método dibuja el cubo de la skybox en el fondo de la escena utilizando los datos de
 * los vértices y la textura cargada. Se asegura de que la skybox no sea recortada y
 * siempre se dibuje detrás de todos los objetos.
 */
void Skybox::render() {
    glDepthFunc(GL_LEQUAL);  // Cambiar la función de profundidad para asegurar que la skybox esté en el fondo

    glBindVertexArray(vao_id);  // Vincular el VAO
    glActiveTexture(GL_TEXTURE0);  // Activar la primera unidad de textura
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);  // Vincular la textura del cubemap
    glDisable(GL_CULL_FACE);  // Desactivar el culling para que todas las caras del cubo sean visibles
    glDrawArrays(GL_TRIANGLES, 0, 36);  // Dibujar las 36 caras del cubo (6 caras * 2 triángulos * 3 vértices)

    glBindVertexArray(0);  // Desvincular el VAO
    glDepthFunc(GL_LESS);  // Restaurar la función de profundidad estándar
}
