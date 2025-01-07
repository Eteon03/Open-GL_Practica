// Este código es de dominio público
// davidbercialblazquez@gmail.com

#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <glm.hpp>
#include <SDL.h>

namespace udit
{

    class Camera
    {
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        float yaw;   // Horizontal rotation
        float pitch; // Vertical rotation
        float speed;
        float sensitivity;

        void update_camera_vectors();

    public:
        Camera(glm::vec3 start_position, glm::vec3 start_up, float start_yaw, float start_pitch);

        void process_keyboard(const Uint8* keystate, float delta_time);
        void process_mouse_motion(float x_offset, float y_offset);
        glm::mat4 get_view_matrix() const;

        void set_speed(float new_speed);
        void set_sensitivity(float new_sensitivity);
    };

}

#endif