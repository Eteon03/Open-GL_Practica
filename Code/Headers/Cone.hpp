// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once

#include <glad/glad.h>
#include <vector>

namespace udit
{

    class Cone
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

        // Arrays din�micos para almacenar los datos del cono:

        std::vector<GLfloat> coordinates;
        std::vector<GLfloat> colors;
        std::vector<GLubyte> indices;

        GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
        GLuint vao_id;                  // Id del VAO del cono

    public:

        Cone(int radial_segments, float radius, float height);
        ~Cone();

        void render();

    };

}