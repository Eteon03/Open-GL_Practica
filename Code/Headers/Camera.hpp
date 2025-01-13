// Este c�digo es de dominio p�blico
// davidbercialblazquez@gmail.com

#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <glm.hpp>  // Biblioteca para operaciones con vectores y matrices en 3D
#include <SDL.h>    // Biblioteca de manejo de eventos y entrada (teclado, rat�n)

namespace udit
{

    /**
     * @class Camera
     * @brief Clase que representa una c�mara en un espacio 3D.
     *
     * Esta clase permite controlar la posici�n y orientaci�n de una c�mara en un entorno 3D,
     * permitiendo el movimiento mediante teclado y la rotaci�n mediante el rat�n.
     * Los par�metros de la c�mara se definen usando �ngulos de rotaci�n (yaw y pitch),
     * y el c�lculo de sus vectores de orientaci�n (front, right, up).
     */
    class Camera
    {
    private:
        glm::vec3 position;  ///< Posici�n de la c�mara en el espacio 3D
        glm::vec3 front;     ///< Direcci�n en la que la c�mara est� mirando
        glm::vec3 up;        ///< Direcci�n "arriba" de la c�mara
        glm::vec3 right;     ///< Direcci�n "derecha" de la c�mara
        glm::vec3 world_up;  ///< Direcci�n global "arriba", usada para calcular los vectores de la c�mara

        float yaw;           ///< Rotaci�n horizontal (alrededor del eje Y)
        float pitch;         ///< Rotaci�n vertical (alrededor del eje X)
        float speed;         ///< Velocidad de movimiento de la c�mara
        float sensitivity;   ///< Sensibilidad del rat�n para controlar la rotaci�n

        /**
         * @brief Actualiza los vectores de direcci�n de la c�mara.
         *
         * Calcula los vectores `front`, `right` y `up` de acuerdo con los �ngulos `yaw` y `pitch`.
         * Estos vectores definen c�mo se mueve y rota la c�mara.
         */
        void update_camera_vectors();

    public:
        /**
         * @brief Constructor de la c�mara.
         *
         * Inicializa la posici�n, los vectores de orientaci�n, los �ngulos de rotaci�n, la velocidad y
         * la sensibilidad de la c�mara. Tambi�n llama al m�todo `update_camera_vectors` para calcular
         * los vectores de la c�mara.
         *
         * @param start_position La posici�n inicial de la c�mara.
         * @param start_up El vector que representa la direcci�n "arriba" global.
         * @param start_yaw El �ngulo de rotaci�n inicial alrededor del eje Y.
         * @param start_pitch El �ngulo de rotaci�n inicial alrededor del eje X.
         */
        Camera(glm::vec3 start_position, glm::vec3 start_up, float start_yaw, float start_pitch);

        /**
         * @brief Procesa la entrada del teclado para mover la c�mara.
         *
         * Mueve la c�mara hacia adelante, atr�s, izquierda o derecha seg�n las teclas presionadas
         * (`W`, `S`, `A`, `D`). La velocidad de movimiento depende del tiempo transcurrido desde
         * el �ltimo frame (`delta_time`).
         *
         * @param keystate El estado actual de las teclas (de SDL).
         * @param delta_time El tiempo transcurrido desde el �ltimo frame.
         */
        void process_keyboard(const Uint8* keystate, float delta_time);

        /**
         * @brief Procesa el movimiento del rat�n para ajustar la orientaci�n de la c�mara.
         *
         * Ajusta los �ngulos de rotaci�n `yaw` (horizontal) y `pitch` (vertical) de la c�mara
         * seg�n el desplazamiento del rat�n. Limita el �ngulo de rotaci�n vertical (`pitch`) para
         * evitar que la c�mara se voltee completamente.
         *
         * @param x_offset Desplazamiento en el eje X del rat�n.
         * @param y_offset Desplazamiento en el eje Y del rat�n.
         */
        void process_mouse_motion(float x_offset, float y_offset);

        /**
         * @brief Obtiene la matriz de vista de la c�mara.
         *
         * Calcula y devuelve la matriz de vista que transforma las coordenadas de la escena
         * al espacio de la c�mara, usando la posici�n y los vectores de orientaci�n de la c�mara.
         *
         * @return glm::mat4 La matriz de vista de la c�mara.
         */
        glm::mat4 get_view_matrix() const;

        /**
         * @brief Establece la velocidad de movimiento de la c�mara.
         *
         * Permite modificar la velocidad de movimiento de la c�mara, ajustando su velocidad
         * al moverla por la escena.
         *
         * @param new_speed La nueva velocidad de movimiento.
         */
        void set_speed(float new_speed);

        /**
         * @brief Establece la sensibilidad del rat�n.
         *
         * Ajusta la sensibilidad del rat�n, controlando cu�n r�pido responden los �ngulos de
         * rotaci�n al mover el rat�n.
         *
         * @param new_sensitivity La nueva sensibilidad del rat�n.
         */
        void set_sensitivity(float new_sensitivity);
    };

}

#endif  // CAMERA_HEADER
