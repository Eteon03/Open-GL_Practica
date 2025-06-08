
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "../Headers/Cube.hpp"

namespace udit
{

    // Vértices de un cubo unitario centrado en el origen
    const GLfloat vertex_positions[] = {
        // Cara +X
        0.5f, -0.5f, -0.5f,   // 0
        0.5f,  0.5f, -0.5f,   // 1
        0.5f,  0.5f,  0.5f,   // 2
        0.5f, -0.5f,  0.5f,   // 3

        // Cara -X
       -0.5f, -0.5f,  0.5f,   // 4
       -0.5f,  0.5f,  0.5f,   // 5
       -0.5f,  0.5f, -0.5f,   // 6
       -0.5f, -0.5f, -0.5f,   // 7

       // Cara +Y
      -0.5f, 0.5f, -0.5f,    // 8
      -0.5f, 0.5f,  0.5f,    // 9
       0.5f, 0.5f,  0.5f,    //10
       0.5f, 0.5f, -0.5f,    //11

       // Cara -Y
      -0.5f, -0.5f,  0.5f,   //12
      -0.5f, -0.5f, -0.5f,   //13
       0.5f, -0.5f, -0.5f,   //14
       0.5f, -0.5f,  0.5f,   //15

       // Cara +Z
      -0.5f, -0.5f, 0.5f,    //16
       0.5f, -0.5f, 0.5f,    //17
       0.5f,  0.5f, 0.5f,    //18
      -0.5f,  0.5f, 0.5f,    //19

      // Cara -Z
      0.5f, -0.5f, -0.5f,   //20
     -0.5f, -0.5f, -0.5f,   //21
     -0.5f,  0.5f, -0.5f,   //22
      0.5f,  0.5f, -0.5f    //23
    };

    // Normales por cara, repetidas por vértice
    const GLfloat vertex_normals[] = {
        // +X
        1,0,0, 1,0,0, 1,0,0, 1,0,0,
        // -X
       -1,0,0,-1,0,0,-1,0,0,-1,0,0,
       // +Y
       0,1,0, 0,1,0, 0,1,0, 0,1,0,
       // -Y
       0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
       // +Z
       0,0,1, 0,0,1, 0,0,1, 0,0,1,
       // -Z
       0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1
    };

    const GLubyte index_data[] =
    {
        0, 1, 2,            // front
        0, 2, 3,
        4, 0, 3,            // left
        4, 3, 7,
        7, 5, 4,            // back
        7, 6, 5,
        1, 5, 6,            // right
        1, 6, 2,
        3, 2, 6,            // top
        3, 6, 7,
        5, 0, 4,            // bottom
        5, 1, 0,
    };


    Cube::Cube()
    {
        // Copiar a los vectores de la clase
        coordinates.assign(vertex_positions, vertex_positions + sizeof(vertex_positions) / sizeof(GLfloat));
        normals.assign(vertex_normals, vertex_normals + sizeof(vertex_normals) / sizeof(GLfloat));
        indices.assign(index_data, index_data + sizeof(index_data) / sizeof(GLubyte));

        // Se generan índices para los VBOs del cubo:

        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);

        // Se activa el VAO del cubo para configurarlo:

        glBindVertexArray (vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de color y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Normales
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Se suben a un EBO los datos de índices:

        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray (0);
    }

    Cube::~Cube()
    {
        // Se liberan los VBOs y el VAO usados:

        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Cube::render ()
    {
        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray (0);
    }
}
