
// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once

#include "../Headers/Scene.hpp"

#include <iostream>
#include <cassert>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr
#include "../Headers/stb_image.h"


namespace udit
{

    using namespace std;

    const string Scene::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        "layout(location = 2) in vec2 vertex_uv;"
        ""
        ""
        "out vec3 front_color;"
        "out vec2 tex_coord;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "   tex_coord = vertex_uv;"
        "}";

    const string Scene::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "in vec2 tex_coord;"
        "uniform sampler2D texture_sampler;"
        "uniform float transparency;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "   vec4 texture_color = texture(texture_sampler, tex_coord);"
        "   fragment_color = vec4(texture_color.rgb, texture_color.a * transparency);"
        "}";

    const std::string Scene::skybox_vertex_shader =
        "#version 330 core\n"
        ""
        "layout (location = 0) in vec3 aPos;"
        "out vec3 TexCoords;"
        "uniform mat4 projection;"
        "uniform mat4 view;"
        ""
        "void main()"
        "{"
        "   TexCoords = aPos;"
        "   vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);"
        "   gl_Position = pos.xyww;"
        "}";

    const string Scene::skybox_fragment_shader =
        "#version 330 core\n"
        ""
        "in vec3 TexCoords;"
        "out vec4 FragColor;"
        "uniform samplerCube skybox;"
        ""
        "void main()"
        "{"
        "   FragColor = texture(skybox, TexCoords);"
        "}";

    Scene::Scene(unsigned width, unsigned height)
        :
        angle(0), plane(12,6), cylinder(10,1,1,3), cone(10,1.4,3),
        camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f),
        skybox({ "../Textures/sky-cube-map-0.png",
            "../Textures/sky-cube-map-1.png",
            "../Textures/sky-cube-map-2.png",
            "../Textures/sky-cube-map-3.png",
            "../Textures/sky-cube-map-4.png",
            "../Textures/sky-cube-map-5.png" }),
        terrain("../Texturas_map/Pavement_Heightmap.jpg", 20.0f, 20.0f, 0.5f) // Ancho, profundidad, altura máxima

    {
        

        // Se establece la configuración básica:

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(.2f, .2f, .2f, 1.f);

        // Se compilan y se activan los shaders:

        program_id = compile_shaders();

        glUseProgram(program_id);

        // Compilar los shaders para el Skybox
        skybox_shader_program = compile_skybox_shaders();

        // Cargar las texturas para la skybox
        GLuint skybox_texture_id = load_skybox_texture({
            "../Textures/sky-cube-map-3.png",//Laterales
            "../Textures/sky-cube-map-1.png",//Posicion cara izquierda
            "../Textures/sky-cube-map-4.png",//Posicion cara inferior
            "../Textures/sky-cube-map-5.png",//Posicion cara superior
            "../Textures/sky-cube-map-2.png",//Laterales
            "../Textures/sky-cube-map-0.png"//Laterales
            });

        skybox.set_texture(skybox_texture_id);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        resize(width, height);

        textureLoader("../Textures/wood_texture.jpg");
        textureLoader("../Textures/cylinder_texture.jpg");
        textureLoader("../Textures/cono_textura.jpg");
        textureLoader("../Texturas_map/Pavement_Albedo.jpg");
        textureLoader("../Textures/hielo_texture.jpg");
        textureLoader("../Textures/purpura.jpg");
    }

    void Scene::process_input(const Uint8* keystate, float delta_time)
    {
        camera.process_keyboard(keystate, delta_time);
    }

    void Scene::process_mouse_motion(float x_offset, float y_offset)
    {
        camera.process_mouse_motion(x_offset, y_offset);
    }

    void Scene::update()
    {
        angle += 0.01f;
        movement_Speed += 0.2f;
    }

    void Scene::render()
    {
        // Parámetros del movimiento circular
        float radius = 7.0f; // Radio de la circunferencia
        float speed = 1.0f;  // Velocidad angular

        // Rotacion entorno a una posicion prestablecida
        float center_x = 2.0f;
        float center_z = -6.0f;

        // Calcular la posición en la circunferencia
        float x = center_x + radius * cos(angle * speed); // Coordenada X
        float z = center_z + radius * sin(angle * speed); // Coordenada Z

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar el Skybox
        glUseProgram(skybox_shader_program);

        // Obtener la matriz de vista de la cámara
        glm::mat4 view_matrix = camera.get_view_matrix();
        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
        
        glUniformMatrix4fv(glGetUniformLocation(skybox_shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(glGetUniformLocation(skybox_shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.get_texture_id());
        skybox.render();

        // Volver a usar el shader normal para los otros objetos
        glUseProgram(program_id);
        glm::mat4 view_matrix_full = camera.get_view_matrix();
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix_full));

        texture_id = 3;
        // Dibujar el plano
        glActiveTexture(GL_TEXTURE0); // Activar la unidad de textura 0
        glBindTexture(GL_TEXTURE_2D, texture_id); // Vincular la textura
        glUniform1i(glGetUniformLocation(program_id, "texture_sampler"), 0);

        glm::mat4 plane_model_matrix(1.0f);
        plane_model_matrix = glm::translate(plane_model_matrix, glm::vec3(-4.f, -0.73f, -9.f));
        plane_model_matrix = glm::rotate(plane_model_matrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 plane_mvp_matrix = view_matrix * plane_model_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(plane_mvp_matrix));
        plane.render();

        texture_id++;

        // Dibujar el cilindro

        glActiveTexture(GL_TEXTURE0); // Activar la unidad de textura 0
        glBindTexture(GL_TEXTURE_2D, texture_id); // Vincular la textura
        glUniform1i(glGetUniformLocation(program_id, "texture_sampler"), 0);

        glm::mat4 cylinder_model_matrix(1.0f);
        cylinder_model_matrix = glm::translate(cylinder_model_matrix, glm::vec3(-2.f, -0.72f, -6.f));
        cylinder_model_matrix = glm::rotate(cylinder_model_matrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 cylinder_mvp_matrix = view_matrix * cylinder_model_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cylinder_mvp_matrix));
        cylinder.render();

        texture_id++;
        // Dibujar el cono 1
        
        glActiveTexture(GL_TEXTURE0); // Activar la unidad de textura 0
        glBindTexture(GL_TEXTURE_2D, texture_id); // Vincular la textura
        glUniform1i(glGetUniformLocation(program_id, "texture_sampler"), 0); // Enviar la textura al shader

        glm::mat4 cone_model_matrix(1.0f);
        cone_model_matrix = glm::translate(cone_model_matrix, glm::vec3(2.f, -0.72f, -6.f));
        cone_model_matrix = glm::rotate(cone_model_matrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
        cone_model_matrix = glm::rotate(cone_model_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 cone_mvp_matrix = view_matrix * cone_model_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone_mvp_matrix));
        cone.render();

        texture_id++;


        //Dibujar el terreno
        glActiveTexture(GL_TEXTURE0); // Activar la unidad de textura 0
        glBindTexture(GL_TEXTURE_2D, texture_id); // Vincular la textura
        glUniform1i(glGetUniformLocation(program_id, "texture_sampler"), 0); // Enviar la textura al shader

        glm::mat4 terrain_model_matrix(1.0f);
        terrain_model_matrix = glm::translate(terrain_model_matrix, glm::vec3(-8.f, -1.12f, -16.f)); // Ajustar posición
        glm::mat4 terrain_mvp_matrix = view_matrix * terrain_model_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(terrain_mvp_matrix));

        terrain.render();

        texture_id++;

        // Dibujar el cono 2
        // Habilitar blending para transparencia
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glUniform1i(glGetUniformLocation(program_id, "texture_sampler"), 0);

        // Establecer transparencia
        GLint transparency_location = glGetUniformLocation(program_id, "transparency");
        glUniform1f(transparency_location, 0.7f); // Ajusta la transparencia (0.0 = totalmente transparente, 1.0 = opaco)

        glm::mat4 cone1_model_matrix(1.0f);
        cone1_model_matrix = glm::translate(cone1_model_matrix, glm::vec3(6.f, 2.3f, -6.f));
        cone1_model_matrix = glm::rotate(cone1_model_matrix, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
        cone1_model_matrix = glm::rotate(cone1_model_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 cone1_mvp_matrix = view_matrix * cone1_model_matrix;

        // Enviar la matriz al shader
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone1_mvp_matrix));
        cone.render();

        // Deshabilitar blending después de renderizar
        glDisable(GL_BLEND);

        texture_id++;

        //Dibuja un tercer cono (peonza)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glUniform1i(glGetUniformLocation(program_id, "texture_sampler"), 0);

        glm::mat4 cone2_model_matrix(1.0f);
        cone2_model_matrix = glm::translate(cone2_model_matrix, glm::vec3(x, 2.3f, z));
        cone2_model_matrix = glm::rotate(cone2_model_matrix, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
        cone2_model_matrix = glm::rotate(cone2_model_matrix, movement_Speed, glm::vec3(0.f, -1.f, 0.f));
        glm::mat4 cone2_mvp_matrix = view_matrix * cone2_model_matrix;

        // Enviar la matriz al shader
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone2_mvp_matrix));
        cone.render();
        
        

    }


    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glViewport(0, 0, width, height);
    }

    void Scene::textureLoader(std::string route)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(route.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            cerr << "Error: No se pudo cargar la textura." << endl;
            return; // En lugar de detener el programa
        }

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Configuración de la textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Cargar los datos en OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Liberar la memoria de la textura cargada
        stbi_image_free(data);

        // Guardar el ID de la textura para usarlo después
        this->texture_id = texture_id;

    }

    GLuint Scene::load_skybox_texture(std::vector<std::string> faces) {
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        int width, height, nr_channels;
        for (GLuint i = 0; i < faces.size(); i++) {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nr_channels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else {
                std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texture_id;
    }

    GLuint Scene::compile_shaders()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Se carga el código de los shaders:

        const char* vertex_shaders_code[] = { vertex_shader_code.c_str() };
        const char* fragment_shaders_code[] = { fragment_shader_code.c_str() };
        const GLint    vertex_shaders_size[] = { (GLint)vertex_shader_code.size() };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        // Se comprueba que si la compilación ha tenido éxito:

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram();

        // Se cargan los shaders compilados en el programa:

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram(program_id);

        // Se comprueba si el linkage ha tenido éxito:

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return (program_id);
    }

    GLuint Scene::compile_skybox_shaders() {
        GLint succeeded = GL_FALSE;

        GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_shader_code[] = { skybox_vertex_shader.c_str() };
        const char* fragment_shader_code[] = { skybox_fragment_shader.c_str() };
        const GLint vertex_shader_size[] = { (GLint)skybox_vertex_shader.size() };
        const GLint fragment_shader_size[] = { (GLint)skybox_fragment_shader.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shader_code, vertex_shader_size);
        glShaderSource(fragment_shader_id, 1, fragment_shader_code, fragment_shader_size);

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        GLuint program_id = glCreateProgram();

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        glLinkProgram(program_id);

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return program_id;
    }


    void Scene::show_compilation_error(GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize(info_log_length);

        glGetShaderInfoLog(shader_id, info_log_length, NULL, &info_log.front());

        cerr << info_log.c_str() << endl;

#ifdef _MSC_VER
        //OutputDebugStringA (info_log.c_str ());
#endif

        assert(false);
    }

    void Scene::show_linkage_error(GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize(info_log_length);

        glGetProgramInfoLog(program_id, info_log_length, NULL, &info_log.front());

        cerr << info_log.c_str() << endl;

#ifdef _MSC_VER
        //OutputDebugStringA (info_log.c_str ());
#endif

        assert(false);
    }

}
