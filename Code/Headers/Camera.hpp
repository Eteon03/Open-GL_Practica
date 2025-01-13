// Este código es de dominio público
// davidbercialblazquez@gmail.com

#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <glm.hpp>  // Biblioteca para operaciones con vectores y matrices en 3D
#include <SDL.h>    // Biblioteca de manejo de eventos y entrada (teclado, ratón)

namespace udit
{

    /**
     * @class Camera
     * @brief Clase que representa una cámara en un espacio 3D.
     *
     * Esta clase permite controlar la posición y orientación de una cámara en un entorno 3D,
     * permitiendo el movimiento mediante teclado y la rotación mediante el ratón.
     * Los parámetros de la cámara se definen usando ángulos de rotación (yaw y pitch),
     * y el cálculo de sus vectores de orientación (front, right, up).
     */
    class Camera
    {
    private:
        glm::vec3 position;  ///< Posición de la cámara en el espacio 3D
        glm::vec3 front;     ///< Dirección en la que la cámara está mirando
        glm::vec3 up;        ///< Dirección "arriba" de la cámara
        glm::vec3 right;     ///< Dirección "derecha" de la cámara
        glm::vec3 world_up;  ///< Dirección global "arriba", usada para calcular los vectores de la cámara

        float yaw;           ///< Rotación horizontal (alrededor del eje Y)
        float pitch;         ///< Rotación vertical (alrededor del eje X)
        float speed;         ///< Velocidad de movimiento de la cámara
        float sensitivity;   ///< Sensibilidad del ratón para controlar la rotación

        /**
         * @brief Actualiza los vectores de dirección de la cámara.
         *
         * Calcula los vectores `front`, `right` y `up` de acuerdo con los ángulos `yaw` y `pitch`.
         * Estos vectores definen cómo se mueve y rota la cámara.
         */
        void update_camera_vectors();

    public:
        /**
         * @brief Constructor de la cámara.
         *
         * Inicializa la posición, los vectores de orientación, los ángulos de rotación, la velocidad y
         * la sensibilidad de la cámara. También llama al método `update_camera_vectors` para calcular
         * los vectores de la cámara.
         *
         * @param start_position La posición inicial de la cámara.
         * @param start_up El vector que representa la dirección "arriba" global.
         * @param start_yaw El ángulo de rotación inicial alrededor del eje Y.
         * @param start_pitch El ángulo de rotación inicial alrededor del eje X.
         */
        Camera(glm::vec3 start_position, glm::vec3 start_up, float start_yaw, float start_pitch);

        /**
         * @brief Procesa la entrada del teclado para mover la cámara.
         *
         * Mueve la cámara hacia adelante, atrás, izquierda o derecha según las teclas presionadas
         * (`W`, `S`, `A`, `D`). La velocidad de movimiento depende del tiempo transcurrido desde
         * el último frame (`delta_time`).
         *
         * @param keystate El estado actual de las teclas (de SDL).
         * @param delta_time El tiempo transcurrido desde el último frame.
         */
        void process_keyboard(const Uint8* keystate, float delta_time);

        /**
         * @brief Procesa el movimiento del ratón para ajustar la orientación de la cámara.
         *
         * Ajusta los ángulos de rotación `yaw` (horizontal) y `pitch` (vertical) de la cámara
         * según el desplazamiento del ratón. Limita el ángulo de rotación vertical (`pitch`) para
         * evitar que la cámara se voltee completamente.
         *
         * @param x_offset Desplazamiento en el eje X del ratón.
         * @param y_offset Desplazamiento en el eje Y del ratón.
         */
        void process_mouse_motion(float x_offset, float y_offset);

        /**
         * @brief Obtiene la matriz de vista de la cámara.
         *
         * Calcula y devuelve la matriz de vista que transforma las coordenadas de la escena
         * al espacio de la cámara, usando la posición y los vectores de orientación de la cámara.
         *
         * @return glm::mat4 La matriz de vista de la cámara.
         */
        glm::mat4 get_view_matrix() const;

        /**
         * @brief Establece la velocidad de movimiento de la cámara.
         *
         * Permite modificar la velocidad de movimiento de la cámara, ajustando su velocidad
         * al moverla por la escena.
         *
         * @param new_speed La nueva velocidad de movimiento.
         */
        void set_speed(float new_speed);

        /**
         * @brief Establece la sensibilidad del ratón.
         *
         * Ajusta la sensibilidad del ratón, controlando cuán rápido responden los ángulos de
         * rotación al mover el ratón.
         *
         * @param new_sensitivity La nueva sensibilidad del ratón.
         */
        void set_sensitivity(float new_sensitivity);
    };

}

#endif  // CAMERA_HEADER
