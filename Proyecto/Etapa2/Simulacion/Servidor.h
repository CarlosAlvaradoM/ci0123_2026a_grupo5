#pragma once
#include "Cola.h"
#include <map>
#include <string>
#include <vector>
#include <atomic>

using namespace std;

/**
 * @brief Nodo final de la cadena que almacena el catálogo de figuras LEGO
 *
 * El Servidor mantiene un mapa de figuras y sus piezas, y responde a las
 * consultas que le reenvía el Tenedor. Nunca se comunica directamente con
 * los clientes: todos los mensajes llegan y salen a través de las dos colas
 * que lo conectan con el Tenedor.
 *
 * Entiende tres comandos del protocolo:
 *  - P/R/dir  → devuelve la lista de figuras disponibles separadas por coma
 *  - P/G/<figura> → devuelve las piezas de esa figura, o P/D/404 si no existe
 *  - P/Q/     → cierre ordenado; confirma con P/Q/ antes de terminar
 *
 * Además puede morir de forma abrupta mediante matar(), que simula una caída
 * real de servidor: inyecta P/K/ en ambas colas para que tanto el hilo del
 * Servidor como el hilo escucha_servidor del Tenedor se enteren y cierren.
 */
class Servidor {
private:
    map<string, vector<string>> figuras;  ///< Catálogo: nombre de figura → lista de piezas
    Cola& entrada;                        ///< Cola por donde llegan las solicitudes del Tenedor
    Cola& salida;                         ///< Cola por donde salen las respuestas hacia el Tenedor
    atomic<bool> vivo;                    ///< Controla si el servidor sigue activo; evita doble muerte

public:
    /**
     * @brief Inicializa el Servidor con el catálogo de figuras precargado
     *
     * Rellena el mapa de figuras con my-melody, pompompurin y cinnamoroll.
     * Las colas se reciben por referencia y deben vivir mientras el servidor corra.
     *
     * @param entrada Cola de la que el servidor leerá las solicitudes
     * @param salida  Cola a la que el servidor escribirá las respuestas
     */
    Servidor(Cola& entrada, Cola& salida);

    /**
     * @brief Simula una caída abrupta del servidor
     *
     * Usa exchange() para garantizar que la muerte sólo ocurra una vez aunque
     * se llame desde varios hilos. Inyecta P/K/ en ambas colas: una para que
     * correr() salga de su bucle, y otra para que el hilo del Tenedor que
     * está esperando respuestas también se despierte y procese la caída.
     */
    void matar();

    /**
     * @brief Bucle principal del servidor; debe ejecutarse en su propio hilo
     *
     * Procesa mensajes en orden hasta recibir P/K/ o P/Q/. Por cada mensaje
     * construye la respuesta adecuada y la pone en la cola de salida para
     * que el Tenedor la reenvíe al cliente correspondiente.
     */
    void correr();
};
