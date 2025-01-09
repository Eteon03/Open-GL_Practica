// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#pragma once

#include <glad/glad.h>
#include <vector>

namespace udit
{

    class Cylinder
    {
    private:

        // �ndices para indexar el array vbo_ids:

        enum
        {
            COORDINATES_VBO,
            COLORS_VBO,
            UVS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        // Arrays din�micos para almacenar los datos del cilindro:

        std::vector<GLfloat> coordinates;
        std::vector<GLfloat> colors;
        std::vector<GLubyte> indices;
        std::vector<GLfloat> uvs;

        GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
        GLuint vao_id;                  // Id del VAO del cilindro

    public:

        Cylinder(int radial_segments, int height_segments, float radius, float height);
        ~Cylinder();

        void render();

    };

}

