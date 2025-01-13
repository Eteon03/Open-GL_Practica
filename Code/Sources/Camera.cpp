// Este código es de dominio público
// davidbercialblazquez@gmail.com

#include "../Headers/Camera.hpp"
#include <gtc/matrix_transform.hpp>

namespace udit
{
    /**
     * @brief Constructor de la clase Camera.
     *
     * Inicializa los valores de la cámara, como la posición, la dirección "arriba",
     * el ángulo de rotación (yaw, pitch), la velocidad de movimiento y la sensibilidad del ratón.
     * Llama al método `update_camera_vectors` para calcular los vectores de dirección.
     *
     * @param start_position La posición inicial de la cámara.
     * @param start_up El vector que define la dirección "arriba" global.
     * @param start_yaw El ángulo de rotación inicial en torno al eje Y.
     * @param start_pitch El ángulo de rotación inicial en torno al eje X.
     */
    Camera::Camera(glm::vec3 start_position, glm::vec3 start_up, float start_yaw, float start_pitch)
        : position(start_position), world_up(start_up), yaw(start_yaw), pitch(start_pitch), speed(5.5f), sensitivity(0.1f)
    {
        update_camera_vectors(); // Llama a la función para actualizar los vectores de la cámara
    }

    /**
     * @brief Actualiza los vectores de la cámara (front, right, up) según los ángulos de rotación (yaw, pitch).
     *
     * Calcula la dirección "frontal" (front) usando los ángulos de rotación (yaw, pitch),
     * y luego calcula los vectores "derecha" (right) y "arriba" (up) mediante el producto cruzado.
     */
    void Camera::update_camera_vectors()
    {
        // Calcular el nuevo vector Front (frontal) en función de los ángulos yaw y pitch
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(new_front);  // Normalizar el vector para mantener su dirección

        // Calcular los vectores Right (derecha) y Up (arriba) usando el producto cruzado
        right = glm::normalize(glm::cross(front, world_up));  // El vector de la derecha es el producto cruzado entre front y world_up
        up = glm::normalize(glm::cross(right, front));  // El vector de arriba es el producto cruzado entre right y front
    }

    /**
     * @brief Procesa la entrada del teclado para mover la cámara.
     *
     * Mueve la cámara en las direcciones especificadas (adelante, atrás, izquierda, derecha),
     * usando las teclas `W`, `S`, `A`, `D`. La velocidad de movimiento depende del tiempo transcurrido
     * entre frames (`delta_time`), lo que asegura un movimiento suave y constante.
     *
     * @param keystate El estado actual de las teclas presionadas (proporcionado por SDL).
     * @param delta_time El tiempo transcurrido entre el último frame, usado para ajustar la velocidad.
     */
    void Camera::process_keyboard(const Uint8* keystate, float delta_time)
    {
        float velocity = speed * delta_time;  // Calcular la velocidad de movimiento basada en el tiempo transcurrido
        if (keystate[SDL_SCANCODE_W])  // Mover hacia adelante (eje Z positivo)
            position += front * velocity;
        if (keystate[SDL_SCANCODE_S])  // Mover hacia atrás (eje Z negativo)
            position -= front * velocity;
        if (keystate[SDL_SCANCODE_A])  // Mover hacia la izquierda (eje X negativo)
            position -= right * velocity;
        if (keystate[SDL_SCANCODE_D])  // Mover hacia la derecha (eje X positivo)
            position += right * velocity;
    }

    /**
     * @brief Procesa el movimiento del ratón para ajustar la orientación de la cámara.
     *
     * Ajusta los ángulos `yaw` (giro horizontal) y `pitch` (giro vertical) basados en el movimiento
     * del ratón, y actualiza los vectores de la cámara en consecuencia. Asegura que el ángulo `pitch` no
     * cause un giro excesivo de la cámara.
     *
     * @param x_offset Desplazamiento del ratón en el eje X (horizontal).
     * @param y_offset Desplazamiento del ratón en el eje Y (vertical).
     */
    void Camera::process_mouse_motion(float x_offset, float y_offset)
    {
        x_offset *= sensitivity;  // Ajusta el desplazamiento por la sensibilidad del ratón
        y_offset *= sensitivity;

        yaw += x_offset;  // Modificar el ángulo de rotación horizontal
        pitch += y_offset;  // Modificar el ángulo de rotación vertical

        // Limitar el ángulo de pitch para evitar que la cámara se voltee completamente
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Actualizar los vectores de la cámara con los nuevos ángulos de rotación
        update_camera_vectors();
    }

    /**
     * @brief Devuelve la matriz de vista de la cámara.
     *
     * La matriz de vista transforma las coordenadas de los objetos de acuerdo a la posición y orientación de la cámara.
     * Utiliza la función `glm::lookAt` para calcular la matriz de vista.
     *
     * @return glm::mat4 La matriz de vista de la cámara.
     */
    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);  // Genera la matriz de vista
    }

    /**
     * @brief Establece la velocidad de movimiento de la cámara.
     *
     * Permite ajustar la velocidad con la que la cámara se mueve en la escena.
     *
     * @param new_speed La nueva velocidad de movimiento de la cámara.
     */
    void Camera::set_speed(float new_speed)
    {
        speed = new_speed;
    }

    /**
     * @brief Establece la sensibilidad del ratón.
     *
     * Permite ajustar la sensibilidad de los movimientos del ratón al modificar los ángulos de la cámara.
     *
     * @param new_sensitivity La nueva sensibilidad del ratón.
     */
    void Camera::set_sensitivity(float new_sensitivity)
    {
        sensitivity = new_sensitivity;
    }

}
