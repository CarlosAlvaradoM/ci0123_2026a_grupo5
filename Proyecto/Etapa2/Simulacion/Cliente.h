#pragma once
#include "Cola.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Cliente que consume figuras de un servidor de tenedor
 * 
 * La clase Cliente representa un cliente que se conecta al sistema para
 * solicitar figuras. Cada cliente tiene su propio ID y se comunica
 * mediante dos colas compartidas con el servidor
 */
class Cliente {
private:
    int id;  ///< Identificador único del cliente
    Cola& salida;  ///< Cola para enviar mensajes al servidor
    Cola& entrada;  ///< Cola para recibir mensajes del servidor

    /**
     * @brief Envía un mensaje al servidor
     * @param msg Contenido del mensaje a enviar
     */
    void enviar(const string& msg);

    /**
     * @brief Espera y recibe una respuesta del servidor
     * @return Contenido del mensaje recibido
     */
    string esperar();

public:
    /**
     * @brief Constructor de la clase Cliente
     * @param id Identificador único del cliente
     * @param salida Referencia a la cola de mensajes de salida
     * @param entrada Referencia a la cola de mensajes de entrada
     */
    Cliente(int id, Cola& salida, Cola& entrada);

    /**
     * @brief Ejecuta la lógica principal del cliente
     * 
     * Realiza el flujo completo de solicitud de figuras:
     * - Solicita la dirección del servidor
     * - Obtiene la lista de figuras disponibles
     * - Solicita una figura específica
     * - Finaliza la sesión
     */
    void correr();
};
