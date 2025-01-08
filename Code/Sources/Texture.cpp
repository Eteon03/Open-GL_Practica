#include "../Headers/Texture.hpp"


Texture::Texture(const std::string& file_path) {
    // Generar y vincular textura
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Configuración de parámetros
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar imagen
    stbi_set_flip_vertically_on_load(true); // Voltea verticalmente la imagen
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Error: No se pudo cargar la textura en " << file_path << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture_id);
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
