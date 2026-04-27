#pragma once
#include "Cola.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Participante que solicita figuras LEGO al sistema distribuido
 *
 * Cada instancia de Cliente representa un usuario independiente que se conecta
 * al Tenedor, consulta qué figuras están disponibles y pide las piezas de una
 * de ellas. La comunicación se hace siempre a través de colas: la cola de salida
 * lleva las solicitudes hacia el Tenedor, y la cola de entrada trae las
 * respuestas de vuelta. Así el cliente nunca habla directamente con el Servidor,
 * ni siquiera sabe que existe.
 *
 * El flujo normal de una sesión es:
 *  1. Enviar P/R/dir para pedir el catálogo de figuras
 *  2. Elegir una figura y enviar P/G/<figura>
 *  3. Cerrar sesión con P/Q/
 *
 * Si en cualquier paso llega P/S/ como respuesta, el Servidor está caído
 * y el cliente termina su sesión de forma ordenada.
 */
class Cliente {
private:
    int id;        ///< Número que identifica a este cliente; también determina qué figura pide
    Cola& salida;  ///< Mensajes que van hacia el Tenedor
    Cola& entrada; ///< Mensajes que vienen del Tenedor hacia este cliente

    /**
     * @brief Empaqueta un texto como Mensaje y lo deja en la cola de salida
     *
     * Asigna automáticamente un req_id incremental para que el mensaje
     * sea rastreable durante todo su recorrido por el sistema.
     *
     * @param msg Contenido del mensaje según el protocolo (ej. "P/R/dir")
     */
    void enviar(const string& msg);

    /**
     * @brief Bloquea el hilo hasta recibir la respuesta del Tenedor
     *
     * Hace pop() sobre la cola de entrada, que es exclusiva de este cliente,
     * por lo que no hay riesgo de leer el mensaje de otro.
     *
     * @return Contenido del mensaje recibido
     */
    string esperar();  ///< @note El original en el repo puede tener void — corrección necesaria para compilar

public:
    /**
     * @brief Construye un cliente listo para operar
     * @param id    Identificador único; se usa también para calcular el retraso inicial
     *              y elegir la figura dentro del catálogo
     * @param salida  Cola compartida a la que el cliente escribe sus solicitudes
     * @param entrada Cola exclusiva de este cliente donde el Tenedor deposita las respuestas
     */
    Cliente(int id, Cola& salida, Cola& entrada);

    /**
     * @brief Ejecuta la sesión completa del cliente en su propio hilo
     *
     * Maneja todos los casos posibles: servidor disponible, servidor caído (P/S/),
     * figura no encontrada (P/D/404) y cierre ordenado. Termina sola cuando
     * la sesión concluye, ya sea con éxito o por error.
     */
    void correr();
};
