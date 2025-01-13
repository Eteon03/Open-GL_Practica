
// Este c�digo es de dominio p�blico
// angel.rodriguez@udit.es
//Modificado por davidbercialblazquez@gmail.com

/**
 * @author Original Author <angel.rodriguez@udit.es>
 * @author davidbercialblazquez@gmail.com
 * 
 * @file Scene.hpp
 * @brief Define la clase Scene y las funciones asociadas.
 *
 * Esta clase maneja la creaci�n, renderizaci�n y la gesti�n de la escena 3D en OpenGL,
 * incluyendo la carga de texturas, la configuraci�n de shaders y la manipulaci�n de objetos 3D.
 */

#pragma once

#include "Cube.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Heightmap.hpp"
#include <string>

namespace udit
{
 /**
 * @class Scene
 * @brief Maneja la creaci�n y renderizaci�n de la escena 3D.
 *
 * Esta clase permite gestionar la escena, incluyendo objetos 3D (como planos, conos, cilindros),
 * c�maras, luces, y texturas. Adem�s, configura y utiliza shaders para la renderizaci�n.
 */
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
        Heightmap terrain;
        float  angle;
        float  movement_Speed;

    public:
     /**
     * @brief Constructor de la escena.
     * @param width Ancho de la ventana de renderizaci�n.
     * @param height Alto de la ventana de renderizaci�n.
     */
        Scene(unsigned width, unsigned height);

     /**
     * @brief Procesa la entrada del teclado para la c�mara.
     * @param keystate Estado actual de las teclas.
     * @param delta_time Tiempo transcurrido desde el �ltimo frame.
     */
        void process_input(const Uint8* keystate, float delta_time);

     /**
     * @brief Procesa el movimiento del rat�n.
     * @param x_offset Desplazamiento en el eje X del rat�n.
     * @param y_offset Desplazamiento en el eje Y del rat�n.
     */

        void process_mouse_motion(float x_offset, float y_offset);

     /**
     * @brief Actualiza el estado de la escena, como la rotaci�n de los objetos.
     */

        void   update();

     /**
     * @brief Renderiza la escena.
     */

        void   render();
     
     /**
     * @brief Cambia el tama�o de la ventana de renderizaci�n.
     * @param width Nuevo ancho de la ventana.
     * @param height Nuevo alto de la ventana.
     */

        void   resize(unsigned width, unsigned height);

     /**
     * @brief Carga una textura desde un archivo.
     * @param route Ruta del archivo de textura.
     */
        void textureLoader(std::string route);

     /**
     * @brief Carga las texturas de la skybox.
     * @param faces Rutas de las im�genes para cada cara de la skybox.
     * @return ID de la textura cargada.
     */

        GLuint load_skybox_texture(std::vector<std::string> faces);

    private:

     /**
     * @brief Compila los shaders del programa de la escena.
     * @return ID del programa de shaders.
     */
        GLuint compile_shaders();
     /**
     * @brief Compila los shaders para la skybox.
     * @return ID del programa de shaders de la skybox.
     */
        GLuint compile_skybox_shaders();
        void   show_compilation_error(GLuint  shader_id);
        void   show_linkage_error(GLuint program_id);

    };

}
