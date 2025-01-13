// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#include "../Headers/Heightmap.hpp"   // Incluir el encabezado de la clase Heightmap
#include <iostream>                    // Incluir la biblioteca para manejar la salida de errores

/**
 * @brief Constructor de la clase Heightmap.
 *
 * Este constructor carga el heightmap desde un archivo, genera la malla correspondiente
 * y crea los buffers de OpenGL para renderizar el terreno.
 *
 * @param heightmap_path Ruta del archivo del heightmap.
 * @param width Ancho de la malla generada.
 * @param depth Profundidad de la malla generada.
 * @param max_height Altura m�xima para escalar los valores de intensidad del heightmap.
 */
Heightmap::Heightmap(const std::string& heightmap_path, float width, float depth, float max_height) {
    // Cargar el heightmap y generar la malla
    load_heightmap(heightmap_path, max_height);
    generate_mesh(width, depth);

    // Crear y configurar buffers de OpenGL
    glGenVertexArrays(1, &vao_id);  // Crear el VAO (Vertex Array Object)
    glGenBuffers(1, &vbo_id);       // Crear el VBO (Vertex Buffer Object)
    glGenBuffers(1, &ebo_id);       // Crear el EBO (Element Buffer Object)

    glBindVertexArray(vao_id);  // Vincular el VAO para configurar sus buffers

    // Enviar datos de v�rtices al VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Configurar los atributos de los v�rtices (posici�n, normal y UV)
    glEnableVertexAttribArray(0);  // Activar el atributo para la posici�n
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);  // Puntero a las posiciones

    glEnableVertexAttribArray(1);  // Activar el atributo para las normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));  // Puntero a las normales

    glEnableVertexAttribArray(2);  // Activar el atributo para las coordenadas UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));  // Puntero a las coordenadas UV

    // Enviar datos de �ndices al EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);  // Desvincular el VAO despu�s de configurarlo
}

/**
 * @brief Destructor de la clase Heightmap.
 *
 * Elimina los recursos de OpenGL utilizados, como el VAO, VBO y EBO.
 */
Heightmap::~Heightmap() {
    glDeleteVertexArrays(1, &vao_id);  // Eliminar el VAO
    glDeleteBuffers(1, &vbo_id);       // Eliminar el VBO
    glDeleteBuffers(1, &ebo_id);       // Eliminar el EBO
}

/**
 * @brief Carga el heightmap desde un archivo y genera los v�rtices correspondientes.
 *
 * Carga una imagen en escala de grises (representando el mapa de alturas), luego escala
 * los valores de los p�xeles para obtener la altura correspondiente. Los v�rtices se
 * calculan a partir de los valores de intensidad de la imagen.
 *
 * @param path Ruta del archivo del heightmap.
 * @param max_height La altura m�xima para los valores del heightmap.
 */
void Heightmap::load_heightmap(const std::string& path, float max_height) {
    int width, height, channels;
    // Cargar la imagen usando stb_image (est� en formato de p�xeles)
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Error: No se pudo cargar el heightmap desde " << path << std::endl;  // Mensaje de error si no se pudo cargar la imagen
        return;
    }

    rows = height;  // Guardar las dimensiones de la imagen
    cols = width;

    // Reservar memoria para los vectores de v�rtices, normales y UVs
    vertices.resize(rows * cols * 8);  // Cada v�rtice tiene posici�n (3), normal (3) y UV (2)
    normals.resize(rows * cols * 3);   // Cada normal tiene 3 componentes
    uvs.resize(rows * cols * 2);       // Cada UV tiene 2 componentes

    // Iterar sobre cada p�xel de la imagen para calcular las posiciones y otros atributos
    for (int z = 0; z < rows; ++z) {
        for (int x = 0; x < cols; ++x) {
            int index = (z * cols + x) * channels;  // �ndice del p�xel actual
            float intensity = data[index] / 255.0f;  // Escalar el valor de intensidad del p�xel
            float y = intensity * max_height;         // Calcular la altura (Y) seg�n la intensidad

            // Calcular los �ndices para el v�rtice, normal y UV
            int vertex_index = (z * cols + x) * 8;
            int normal_index = (z * cols + x) * 3;
            int uv_index = (z * cols + x) * 2;

            // Guardar la posici�n (X, Y, Z) del v�rtice
            vertices[vertex_index + 0] = static_cast<float>(x);
            vertices[vertex_index + 1] = y;
            vertices[vertex_index + 2] = static_cast<float>(z);

            // Guardar la normal (por ahora es una normal simple que puede ser calculada m�s tarde)
            vertices[vertex_index + 3] = 0.0f;
            vertices[vertex_index + 4] = 1.0f;
            vertices[vertex_index + 5] = 0.0f;

            // Guardar las coordenadas UV
            vertices[vertex_index + 6] = static_cast<float>(x) / (cols - 1);
            vertices[vertex_index + 7] = static_cast<float>(z) / (rows - 1);
        }
    }

    stbi_image_free(data);  // Liberar la memoria de la imagen cargada
}

/**
 * @brief Genera la malla del heightmap a partir de los v�rtices.
 *
 * Crea los �ndices necesarios para renderizar el terreno utilizando tri�ngulos.
 *
 * @param width Ancho de la malla.
 * @param depth Profundidad de la malla.
 */
void Heightmap::generate_mesh(float width, float depth) {
    float x_scale = width / (cols - 1);  // Escalar los v�rtices en el eje X
    float z_scale = depth / (rows - 1);  // Escalar los v�rtices en el eje Z

    indices.reserve((rows - 1) * (cols - 1) * 6);  // Reservar espacio para los �ndices (2 tri�ngulos por celda)

    // Crear los �ndices para los tri�ngulos
    for (int z = 0; z < rows; ++z) {
        for (int x = 0; x < cols; ++x) {
            // Escalar las coordenadas de los v�rtices
            int vertex_index = (z * cols + x) * 8;
            vertices[vertex_index + 0] *= x_scale;
            vertices[vertex_index + 2] *= z_scale;

            // Crear los �ndices para los tri�ngulos (2 tri�ngulos por celda)
            if (x < cols - 1 && z < rows - 1) {
                int top_left = z * cols + x;
                int top_right = top_left + 1;
                int bottom_left = (z + 1) * cols + x;
                int bottom_right = bottom_left + 1;

                indices.push_back(top_left);
                indices.push_back(bottom_left);
                indices.push_back(top_right);

                indices.push_back(top_right);
                indices.push_back(bottom_left);
                indices.push_back(bottom_right);
            }
        }
    }
}

/**
 * @brief Renderiza el heightmap en la escena.
 *
 * Este m�todo dibuja la malla generada utilizando los datos almacenados en los buffers de OpenGL.
 */
void Heightmap::render() {
    glBindVertexArray(vao_id);  // Vincular el VAO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);  // Dibujar los tri�ngulos
    glBindVertexArray(0);  // Desvincular el VAO
}
