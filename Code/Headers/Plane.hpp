// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

// Plane.hpp

#pragma once

#include <glad/glad.h>
#include <vector>

namespace udit
{

    class Plane
    {
    private:

        // �ndices para indexar el array vbo_ids:

        enum
        {
            COORDINATES_VBO,
            COLORS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        // Arrays din�micos para almacenar los datos del plano:

        std::vector<GLfloat> coordinates;
        std::vector<GLfloat> colors;
        std::vector<GLubyte> indices;

        GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
        GLuint vao_id;                  // Id del VAO del plano

    public:

        Plane(int width, int height);
        ~Plane();

        void render();

    };

}
