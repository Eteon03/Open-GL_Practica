// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#include "../Headers/Camera.hpp"
#include <gtc/matrix_transform.hpp>

namespace udit
{
    /**
     * @brief Constructor de la clase Camera.
     *
     * Inicializa los valores de la c�mara, como la posici�n, la direcci�n "arriba",
     * el �ngulo de rotaci�n (yaw, pitch), la velocidad de movimiento y la sensibilidad del rat�n.
     * Llama al m�todo `update_camera_vectors` para calcular los vectores de direcci�n.
     *
     * @param start_position La posici�n inicial de la c�mara.
     * @param start_up El vector que define la direcci�n "arriba" global.
     * @param start_yaw El �ngulo de rotaci�n inicial en torno al eje Y.
     * @param start_pitch El �ngulo de rotaci�n inicial en torno al eje X.
     */
    Camera::Camera(glm::vec3 start_position, glm::vec3 start_up, float start_yaw, float start_pitch)
        : position(start_position), world_up(start_up), yaw(start_yaw), pitch(start_pitch), speed(5.5f), sensitivity(0.1f)
    {
        update_camera_vectors(); // Llama a la funci�n para actualizar los vectores de la c�mara
    }

    /**
     * @brief Actualiza los vectores de la c�mara (front, right, up) seg�n los �ngulos de rotaci�n (yaw, pitch).
     *
     * Calcula la direcci�n "frontal" (front) usando los �ngulos de rotaci�n (yaw, pitch),
     * y luego calcula los vectores "derecha" (right) y "arriba" (up) mediante el producto cruzado.
     */
    void Camera::update_camera_vectors()
    {
        // Calcular el nuevo vector Front (frontal) en funci�n de los �ngulos yaw y pitch
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(new_front);  // Normalizar el vector para mantener su direcci�n

        // Calcular los vectores Right (derecha) y Up (arriba) usando el producto cruzado
        right = glm::normalize(glm::cross(front, world_up));  // El vector de la derecha es el producto cruzado entre front y world_up
        up = glm::normalize(glm::cross(right, front));  // El vector de arriba es el producto cruzado entre right y front
    }

    /**
     * @brief Procesa la entrada del teclado para mover la c�mara.
     *
     * Mueve la c�mara en las direcciones especificadas (adelante, atr�s, izquierda, derecha),
     * usando las teclas `W`, `S`, `A`, `D`. La velocidad de movimiento depende del tiempo transcurrido
     * entre frames (`delta_time`), lo que asegura un movimiento suave y constante.
     *
     * @param keystate El estado actual de las teclas presionadas (proporcionado por SDL).
     * @param delta_time El tiempo transcurrido entre el �ltimo frame, usado para ajustar la velocidad.
     */
    void Camera::process_keyboard(const Uint8* keystate, float delta_time)
    {
        float velocity = speed * delta_time;  // Calcular la velocidad de movimiento basada en el tiempo transcurrido
        if (keystate[SDL_SCANCODE_W])  // Mover hacia adelante (eje Z positivo)
            position += front * velocity;
        if (keystate[SDL_SCANCODE_S])  // Mover hacia atr�s (eje Z negativo)
            position -= front * velocity;
        if (keystate[SDL_SCANCODE_A])  // Mover hacia la izquierda (eje X negativo)
            position -= right * velocity;
        if (keystate[SDL_SCANCODE_D])  // Mover hacia la derecha (eje X positivo)
            position += right * velocity;
    }

    /**
     * @brief Procesa el movimiento del rat�n para ajustar la orientaci�n de la c�mara.
     *
     * Ajusta los �ngulos `yaw` (giro horizontal) y `pitch` (giro vertical) basados en el movimiento
     * del rat�n, y actualiza los vectores de la c�mara en consecuencia. Asegura que el �ngulo `pitch` no
     * cause un giro excesivo de la c�mara.
     *
     * @param x_offset Desplazamiento del rat�n en el eje X (horizontal).
     * @param y_offset Desplazamiento del rat�n en el eje Y (vertical).
     */
    void Camera::process_mouse_motion(float x_offset, float y_offset)
    {
        x_offset *= sensitivity;  // Ajusta el desplazamiento por la sensibilidad del rat�n
        y_offset *= sensitivity;

        yaw += x_offset;  // Modificar el �ngulo de rotaci�n horizontal
        pitch += y_offset;  // Modificar el �ngulo de rotaci�n vertical

        // Limitar el �ngulo de pitch para evitar que la c�mara se voltee completamente
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Actualizar los vectores de la c�mara con los nuevos �ngulos de rotaci�n
        update_camera_vectors();
    }

    /**
     * @brief Devuelve la matriz de vista de la c�mara.
     *
     * La matriz de vista transforma las coordenadas de los objetos de acuerdo a la posici�n y orientaci�n de la c�mara.
     * Utiliza la funci�n `glm::lookAt` para calcular la matriz de vista.
     *
     * @return glm::mat4 La matriz de vista de la c�mara.
     */
    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);  // Genera la matriz de vista
    }

    /**
     * @brief Establece la velocidad de movimiento de la c�mara.
     *
     * Permite ajustar la velocidad con la que la c�mara se mueve en la escena.
     *
     * @param new_speed La nueva velocidad de movimiento de la c�mara.
     */
    void Camera::set_speed(float new_speed)
    {
        speed = new_speed;
    }

    /**
     * @brief Establece la sensibilidad del rat�n.
     *
     * Permite ajustar la sensibilidad de los movimientos del rat�n al modificar los �ngulos de la c�mara.
     *
     * @param new_sensitivity La nueva sensibilidad del rat�n.
     */
    void Camera::set_sensitivity(float new_sensitivity)
    {
        sensitivity = new_sensitivity;
    }

}
