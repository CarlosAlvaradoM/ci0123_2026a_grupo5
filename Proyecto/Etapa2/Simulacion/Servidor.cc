#include "Servidor.h"
#include <sstream>
#include <mutex>
#include <iostream>

using namespace std;

extern mutex console_mtx;
extern void logger(const string&, const string&);

Servidor::Servidor(Cola& entrada, Cola& salida)
    : entrada(entrada), salida(salida), vivo(true) {
    figuras["my-melody"] = {
        "brick 2x4 (rosa)", "brick 1x2 (blanco)", "placa 2x2 (rosa)",
        "brick 1x1 (negro)", "brick 1x1 (rosa)", "placa 1x2 (blanco)"
    };
    figuras["pompompurin"] = {
        "brick 2x4 (amarillo)", "brick 1x2 (marron)", "placa 2x2 (amarillo)",
        "brick 1x1 (negro)", "brick 1x4 (amarillo)", "placa 1x2 (marron)"
    };
    figuras["cinnamoroll"] = {
        "brick 2x4 (blanco)", "brick 1x2 (azul)", "placa 2x2 (blanco)",
        "brick 1x1 (negro)", "brick 1x1 (azul)", "placa 2x4 (blanco)"
    };
}

void Servidor::matar() {
    // exchange() devuelve el valor anterior; si ya era false alguien se nos adelantó
    // y no hay que hacer nada más
    if (!vivo.exchange(false)) return;

    logger("SERVIDOR", "P/K/ - Anunciando muerte al Tenedor");

    // P/K/ en la cola de salida despierta al hilo escucha_servidor del Tenedor
    Mensaje kill_salida;
    kill_salida.id_cliente = -1;
    kill_salida.contenido  = "P/K/";
    kill_salida.req_id     = "kill";
    salida.push(kill_salida);

    // P/K/ en la cola de entrada despierta al propio bucle de correr()
    // para que el servidor no quede bloqueado esperando la próxima solicitud
    Mensaje kill_entrada;
    kill_entrada.id_cliente = -1;
    kill_entrada.contenido  = "P/K/";
    kill_entrada.req_id     = "kill";
    entrada.push(kill_entrada);
}

void Servidor::correr() {
    logger("SERVIDOR", "P/S/172.16.123.71:8080 - En linea, esperando solicitudes");

    while (true) {
        Mensaje m = entrada.pop();

        if (m.contenido == "P/K/") {
            logger("SERVIDOR", "P/K/ - Muerte activada, dejando de atender");
            break;
        }

        // P/Q/ sin id_cliente real significa cierre ordenado pedido por el Tenedor
        if (m.contenido == "P/Q/") {
            logger("SERVIDOR", "P/Q/ - Cierre ordenado, terminando");
            Mensaje resp;
            resp.id_cliente = -1;
            resp.contenido  = "P/Q/";
            resp.req_id     = m.req_id;
            salida.push(resp);
            break;
        }

        logger("SERVIDOR", "[" + m.req_id + "] Recibido de cliente " +
               to_string(m.id_cliente) + ": " + m.contenido);

        Mensaje respuesta;
        respuesta.id_cliente = m.id_cliente;
        respuesta.req_id     = m.req_id;

        if (m.contenido == "P/R/dir") {
            // Armar la lista de figuras como "P/D/figura1,figura2,..."
            string lista = "P/D/";
            bool primero = true;
            for (auto& par : figuras) {
                if (!primero) lista += ",";
                lista += par.first;
                primero = false;
            }
            respuesta.contenido = lista;
            logger("SERVIDOR", "[" + m.req_id + "] Enviando directorio: " + respuesta.contenido);

        } else if (m.contenido.size() > 4 && m.contenido.substr(0, 4) == "P/G/") {
            string figura = m.contenido.substr(4);
            if (figuras.count(figura)) {
                // Armar la lista de piezas como "P/D/pieza1,pieza2,..."
                string piezas = "P/D/";
                bool primero = true;
                for (auto& pieza : figuras[figura]) {
                    if (!primero) piezas += ",";
                    piezas += pieza;
                    primero = false;
                }
                respuesta.contenido = piezas;
                logger("SERVIDOR", "[" + m.req_id + "] Enviando piezas de " + figura);
            } else {
                respuesta.contenido = "P/D/404";
                logger("SERVIDOR", "[" + m.req_id + "] P/D/404 - Figura no encontrada: " + figura);
            }
        } else {
            respuesta.contenido = "P/D/400";
            logger("SERVIDOR", "[" + m.req_id + "] P/D/400 - Comando no reconocido: " + m.contenido);
        }

        salida.push(respuesta);
    }

    logger("SERVIDOR", "Servidor terminado");
}
