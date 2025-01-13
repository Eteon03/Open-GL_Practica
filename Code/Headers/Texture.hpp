// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once  // Prevenir la inclusi�n m�ltiple del archivo de encabezado
#include <string>        // Biblioteca para trabajar con cadenas de texto
#include <glad/glad.h>   // Biblioteca para cargar funciones de OpenGL
#include "stb_image.h"   // Librer�a para cargar im�genes
#include <iostream>      // Para imprimir mensajes en consola

/**
 * @class Texture
 * @brief Clase que representa una textura en OpenGL.
 *
 * Esta clase maneja la carga, activaci�n, desactivaci�n y liberaci�n de una textura
 * en OpenGL. Utiliza la librer�a stb_image para cargar im�genes y convertirlas en
 * texturas que puedan ser utilizadas en la GPU.
 */
class Texture {
public:
    /**
     * @brief Constructor de la clase Texture.
     *
     * Este constructor carga una imagen desde un archivo de ruta especificada y
     * la convierte en una textura que puede ser utilizada en OpenGL.
     *
     * @param file_path Ruta del archivo de la imagen a cargar como textura.
     */
    Texture(const std::string& file_path);

    /**
     * @brief Destructor de la clase Texture.
     *
     * Elimina la textura de OpenGL liberando los recursos asociados a ella.
     */
    ~Texture();

    /**
     * @brief Activa la textura para su uso en una unidad de textura espec�fica.
     *
     * Este m�todo vincula la textura a la unidad de textura especificada para que
     * pueda ser utilizada en las operaciones de renderizado.
     *
     * @param unit N�mero de unidad de textura (por defecto es 0).
     */
    void bind(unsigned int unit = 0) const;

    /**
     * @brief Desactiva la textura.
     *
     * Este m�todo desvincula la textura actualmente activa.
     */
    void unbind() const;

private:
    GLuint texture_id;  ///< ID de la textura en OpenGL.
    int width, height, channels;  ///< Dimensiones de la imagen cargada y el n�mero de canales de color.
};

