#pragma once
#include "Cola.h"
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <map>

using namespace std;

/**
 * @brief Intermediario que conecta a los clientes con el Servidor de figuras
 *
 * El Tenedor actúa como proxy: recibe mensajes de múltiples clientes,
 * los reenvía al Servidor y distribuye las respuestas de vuelta al cliente
 * correcto. Para lograrlo sin bloqueos innecesarios, lanza un hilo por cada
 * cliente más un hilo extra dedicado a escuchar al Servidor.
 *
 * La tabla @c pendientes lleva el registro de qué req_id pertenece a qué
 * cliente mientras la respuesta está en tránsito. Cuando el Servidor responde,
 * el Tenedor consulta esa tabla para saber a cuál cola de respuesta debe
 * entregar el mensaje.
 *
 * Si el Servidor muere (P/K/), el hilo escucha_servidor lo detecta, pone
 * @c servidor_vivo a false, responde P/S/ a todas las solicitudes que quedaron
 * pendientes y se cierra. Los hilos de atención a clientes comprueban esa
 * bandera antes de reenviar cada solicitud, así que cualquier mensaje posterior
 * a la caída también recibe P/S/ de inmediato.
 */
class Tenedor {
private:
    vector<Cola*>& colas_entrada;   ///< Una cola de entrada por cliente; el cliente escribe aquí
    Cola& salida_servidor;          ///< Canal de salida hacia el Servidor
    Cola& entrada_servidor;         ///< Canal de entrada desde el Servidor
    vector<Cola*>& colas_respuesta; ///< Una cola de respuesta por cliente; el Tenedor escribe aquí

    int num_clientes;               ///< Cantidad de clientes que se espera que se conecten

    mutex mtx_servidor;             ///< Protege servidor_vivo y pendientes de accesos concurrentes
    atomic<bool> servidor_vivo;     ///< true mientras el Servidor no ha enviado P/K/
    map<string, int> pendientes;    ///< req_id → id_cliente para respuestas en tránsito

    /**
     * @brief Hilo que lee continuamente las respuestas del Servidor
     *
     * Usa pop_timeout() para no quedarse bloqueado indefinidamente y poder
     * reaccionar cuando servidor_vivo cambia a false. Cada respuesta que llega
     * se saca de pendientes y se deposita en la cola del cliente correspondiente.
     * Termina al recibir P/K/ o P/Q/.
     */
    void escuchar_servidor();

    /**
     * @brief Hilo dedicado a un cliente específico
     *
     * Lee solicitudes de la cola de entrada de ese cliente y las reenvía al
     * Servidor siempre que esté vivo. Si el Servidor ya cayó, responde P/S/
     * directamente sin intentar el reenvío. Termina cuando el cliente manda P/Q/.
     *
     * @param id_cliente Índice del cliente al que atiende este hilo
     */
    void atender_cliente(int id_cliente);

public:
    /**
     * @brief Construye el Tenedor con todas sus colas de comunicación
     *
     * @param colas_entrada    Vector de colas; la posición i corresponde al cliente i
     * @param salida_servidor  Cola hacia el Servidor
     * @param entrada_servidor Cola desde el Servidor
     * @param colas_respuesta  Vector de colas de respuesta; la posición i es del cliente i
     * @param num_clientes     Número total de clientes que participan en la simulación
     */
    Tenedor(vector<Cola*>& colas_entrada, Cola& salida_servidor,
            Cola& entrada_servidor, vector<Cola*>& colas_respuesta,
            int num_clientes);

    /**
     * @brief Arranca todos los hilos internos y espera a que terminen
     *
     * Lanza escuchar_servidor() y un hilo atender_cliente() por cada cliente.
     * Cuando todos los hilos de clientes terminan, envía P/Q/ al Servidor
     * si todavía está vivo, y luego espera a que escuchar_servidor() cierre.
     */
    void correr();
};
