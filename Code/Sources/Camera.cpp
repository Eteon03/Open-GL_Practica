// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Camera.hpp"
#include <gtc/matrix_transform.hpp>

namespace udit
{

    Camera::Camera(glm::vec3 start_position, glm::vec3 start_up, float start_yaw, float start_pitch)
        : position(start_position), world_up(start_up), yaw(start_yaw), pitch(start_pitch), speed(2.5f), sensitivity(0.1f)
    {
        update_camera_vectors();
    }

    void Camera::update_camera_vectors()
    {
        // Calculate the new Front vector
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(new_front);

        // Recalculate Right and Up vector
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }

    void Camera::process_keyboard(const Uint8* keystate, float delta_time)
    {
        float velocity = speed * delta_time;
        if (keystate[SDL_SCANCODE_W])
            position += front * velocity;
        if (keystate[SDL_SCANCODE_S])
            position -= front * velocity;
        if (keystate[SDL_SCANCODE_A])
            position -= right * velocity;
        if (keystate[SDL_SCANCODE_D])
            position += right * velocity;
    }

    void Camera::process_mouse_motion(float x_offset, float y_offset)
    {
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Update Front, Right and Up Vectors using the updated Euler angles
        update_camera_vectors();
    }

    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::set_speed(float new_speed)
    {
        speed = new_speed;
    }

    void Camera::set_sensitivity(float new_sensitivity)
    {
        sensitivity = new_sensitivity;
    }

}