// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Texture.hpp"  // Incluir el encabezado de la clase Texture

/**
 * @brief Constructor de la clase Texture.
 *
 * Este constructor carga una imagen desde un archivo especificado y la convierte en
 * una textura en OpenGL. Además, configura los parámetros de la textura y genera
 * los mipmaps necesarios.
 *
 * @param file_path Ruta del archivo de la imagen a cargar como textura.
 */
Texture::Texture(const std::string& file_path) {
    // Generar y vincular una nueva textura en OpenGL
    glGenTextures(1, &texture_id);  // Generar el ID de la textura
    glBindTexture(GL_TEXTURE_2D, texture_id);  // Vincular la textura como una textura 2D

    // Configuración de parámetros de la textura:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Configurar el modo de envolvimiento en el eje S (horizontal)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // Configurar el modo de envolvimiento en el eje T (vertical)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Establecer el filtro para la textura al reducirla (minimización)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // Establecer el filtro para la textura al ampliarla (maximización)

    // Cargar la imagen utilizando stb_image y configurar la textura en OpenGL
    stbi_set_flip_vertically_on_load(true);  // Voltear la imagen verticalmente al cargarla (para ajustarse al sistema de coordenadas de OpenGL)
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);  // Cargar la imagen desde el archivo

    if (data) {
        // Determinar el formato de la textura en función de los canales (RGB o RGBA)
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;  // Si la imagen tiene 4 canales, se usa GL_RGBA, de lo contrario GL_RGB

        // Crear la textura 2D en OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Generar mipmaps para la textura (optimización de la calidad según el nivel de zoom)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        // Si no se pudo cargar la imagen, imprimir un mensaje de error
        std::cerr << "Error: No se pudo cargar la textura en " << file_path << std::endl;
    }

    // Liberar la memoria utilizada por la imagen cargada
    stbi_image_free(data);
}

/**
 * @brief Destructor de la clase Texture.
 *
 * Elimina la textura de OpenGL y libera los recursos asociados a ella.
 */
Texture::~Texture() {
    glDeleteTextures(1, &texture_id);  // Eliminar la textura de OpenGL
}

/**
 * @brief Vincula la textura a una unidad de textura específica.
 *
 * Este método activa la unidad de textura especificada y vincula la textura de OpenGL
 * para que pueda ser utilizada en los shaders durante el renderizado.
 *
 * @param unit La unidad de textura que se debe activar. El valor por defecto es 0.
 */
void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);  // Activar la unidad de textura especificada
    glBindTexture(GL_TEXTURE_2D, texture_id);  // Vincular la textura a la unidad activa
}

/**
 * @brief Desvincula la textura actualmente activa.
 *
 * Este método desvincula cualquier textura actualmente activa para asegurarse de que
 * no se use en el siguiente renderizado.
 */
void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);  // Desvincular la textura 2D
}
