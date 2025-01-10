
// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once

#include "Cube.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include <string>

namespace udit
{

    class Scene
    {
    private:

        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;
        static const std::string skybox_vertex_shader;
        static const std::string skybox_fragment_shader;

        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;

        Cube   cube;
        Plane plane;
        Cylinder cylinder;
        Cone cone;
        Camera camera;
        GLuint texture_id;
        GLuint program_id;
        Skybox skybox;
        GLuint skybox_shader_program;
        float  angle;

    public:

        Scene(unsigned width, unsigned height);

        void process_input(const Uint8* keystate, float delta_time); // Procesar teclado
        void process_mouse_motion(float x_offset, float y_offset);  // Procesar ratón
        void   update();
        void   render();
        void   resize(unsigned width, unsigned height);
        void textureLoader(std::string route);
        GLuint load_skybox_texture(std::vector<std::string> faces);

    private:

        GLuint compile_shaders();
        GLuint compile_skybox_shaders();
        void   show_compilation_error(GLuint  shader_id);
        void   show_linkage_error(GLuint program_id);

    };

}
