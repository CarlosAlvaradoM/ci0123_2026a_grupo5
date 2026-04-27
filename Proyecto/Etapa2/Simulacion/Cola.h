#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <optional>

using namespace std;

/**
 * @brief Unidad mínima de comunicación entre componentes del sistema
 *
 * Cada mensaje que viaja entre un cliente, el Tenedor y el Servidor
 * se empaqueta en esta estructura. El campo req_id permite rastrear
 * una solicitud de punta a punta aunque pase por varios hilos.
 */
struct Mensaje {
    int id_cliente;    ///< ID del cliente que originó el mensaje; -1 para mensajes del sistema (P/K/, P/Q/)
    string contenido;  ///< Cuerpo del mensaje según el protocolo (ej. "P/R/dir", "P/G/my-melody")
    string req_id;     ///< Identificador único de la solicitud para correlacionar respuestas
};

/**
 * @brief Cola thread-safe para el intercambio de mensajes entre hilos
 *
 * Envuelve una std::queue con un mutex y una variable de condición para que
 * varios hilos puedan producir y consumir mensajes sin carreras de datos.
 * El método pop() bloquea al consumidor hasta que haya algo que leer,
 * y pop_timeout() hace lo mismo pero con límite de tiempo para que el
 * llamador pueda reaccionar si el otro extremo tardó demasiado.
 */
class Cola {
private:
    queue<Mensaje> datos;   ///< Almacenamiento interno de los mensajes pendientes
    mutex mtx;              ///< Protege el acceso concurrente a datos
    condition_variable cv;  ///< Notifica a los consumidores cuando llega un mensaje nuevo

public:
    /**
     * @brief Inserta un mensaje al final de la cola y despierta a un consumidor
     * @param m Mensaje a encolar
     */
    void push(const Mensaje& m) {
        unique_lock<mutex> lock(mtx);
        datos.push(m);
        cv.notify_one();
    }

    /**
     * @brief Extrae el primer mensaje de la cola, bloqueando si está vacía
     *
     * El hilo llamador queda suspendido hasta que otro hilo haga push().
     * No tiene tiempo límite, así que se usa cuando se sabe con certeza
     * que el mensaje va a llegar (por ejemplo, esperar la respuesta del servidor
     * cuando se sabe que está vivo).
     *
     * @return Mensaje el mensaje más antiguo de la cola
     */
    Mensaje pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]{ return !datos.empty(); });
        Mensaje m = datos.front();
        datos.pop();
        return m;
    }

    /**
     * @brief Intenta extraer un mensaje esperando como máximo el tiempo indicado
     *
     * Útil en el Tenedor para detectar cuando el servidor dejó de responder:
     * si la espera se agota y el servidor debería haber contestado, algo salió mal.
     *
     * @param ms Tiempo máximo de espera en milisegundos
     * @return optional<Mensaje> El mensaje si llegó a tiempo, o nullopt si se agotó el plazo
     */
    optional<Mensaje> pop_timeout(int ms) {
        unique_lock<mutex> lock(mtx);
        bool llego = cv.wait_for(lock, chrono::milliseconds(ms),
                                 [this]{ return !datos.empty(); });
        if (!llego) return nullopt;
        Mensaje m = datos.front();
        datos.pop();
        return m;
    }

    /**
     * @brief Indica si la cola no tiene mensajes en este momento
     * @return true si la cola está vacía, false en caso contrario
     */
    bool vacia() {
        unique_lock<mutex> lock(mtx);
        return datos.empty();
    }
};
