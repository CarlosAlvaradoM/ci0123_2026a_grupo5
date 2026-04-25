#include "Tenedor.h"
#include <thread>
#include <iostream>
#include <atomic>

using namespace std;

extern mutex console_mtx;
extern void logger(const string&, const string&);

static const int TIMEOUT_SERVIDOR_MS = 300;

Tenedor::Tenedor(vector<Cola*>& colas_entrada, Cola& salida_servidor,
                 Cola& entrada_servidor, vector<Cola*>& colas_respuesta,
                 int num_clientes)
    : colas_entrada(colas_entrada),
      salida_servidor(salida_servidor),
      entrada_servidor(entrada_servidor),
      colas_respuesta(colas_respuesta),
      num_clientes(num_clientes),
      servidor_vivo(true) {}

void Tenedor::escuchar_servidor() {
    logger("TENEDOR", "Hilo escucha_servidor iniciado");

    while (true) {
        auto resultado = entrada_servidor.pop_timeout(TIMEOUT_SERVIDOR_MS);

        if (!resultado) {
            if (!servidor_vivo) break;
            continue;
        }

        Mensaje resp = *resultado;

        if (resp.contenido == "P/K/") {
            logger("TENEDOR", "P/K/ recibido - Servidor caido, marcando como no disponible");

            lock_guard<mutex> lock(mtx_servidor);
            servidor_vivo = false;

            for (auto& par : pendientes) {
                logger("TENEDOR", "[" + par.first + "] P/S/ → Sin servidor para cliente " +
                       to_string(par.second));
                Mensaje sin_srv;
                sin_srv.id_cliente = par.second;
                sin_srv.contenido  = "P/S/";
                sin_srv.req_id     = par.first;
                colas_respuesta[par.second]->push(sin_srv);
            }
            pendientes.clear();
            break;
        }

        if (resp.contenido == "P/Q/" && resp.id_cliente == -1) {
            logger("TENEDOR", "Servidor cerrado ordenadamente");
            break;
        }

        logger("TENEDOR", "[" + resp.req_id + "] Respuesta del servidor para cliente " +
               to_string(resp.id_cliente) + ": " + resp.contenido);
        {
            lock_guard<mutex> lock(mtx_servidor);
            pendientes.erase(resp.req_id);
        }
        colas_respuesta[resp.id_cliente]->push(resp);
    }

    logger("TENEDOR", "Hilo escucha_servidor terminado");
}

void Tenedor::atender_cliente(int id_cliente) {
    logger("TENEDOR", "Hilo C" + to_string(id_cliente) + "-F-S iniciado");

    while (true) {
        Mensaje m = colas_entrada[id_cliente]->pop();

        logger("TENEDOR", "[" + m.req_id + "] C" + to_string(id_cliente) + " → Tenedor: " + m.contenido);

        if (m.contenido == "P/Q/") {
            logger("TENEDOR", "[" + m.req_id + "] P/Q/ de cliente " + to_string(id_cliente) + " - Respondiendo P/A/");
            Mensaje ack;
            ack.id_cliente = id_cliente;
            ack.contenido  = "P/A/";
            ack.req_id     = m.req_id;
            colas_respuesta[id_cliente]->push(ack);
            break;
        }

        {
            lock_guard<mutex> lock(mtx_servidor);

            if (!servidor_vivo) {
                logger("TENEDOR", "[" + m.req_id + "] P/S/ → Sin servidor para cliente " + to_string(id_cliente));
                Mensaje sin_srv;
                sin_srv.id_cliente = id_cliente;
                sin_srv.contenido  = "P/S/";
                sin_srv.req_id     = m.req_id;
                colas_respuesta[id_cliente]->push(sin_srv);
                continue;
            }

            pendientes[m.req_id] = id_cliente;
            logger("TENEDOR", "[" + m.req_id + "] Tenedor → Servidor: " + m.contenido);
            salida_servidor.push(m);
        }
    }

    logger("TENEDOR", "Hilo C" + to_string(id_cliente) + "-F-S terminado");
}

void Tenedor::correr() {
    logger("TENEDOR", "P/C/172.16.123.70:8080 - En linea");

    thread hilo_escucha(&Tenedor::escuchar_servidor, this);

    vector<thread> hilos_clientes;
    for (int i = 0; i < num_clientes; i++) {
        hilos_clientes.emplace_back(&Tenedor::atender_cliente, this, i);
    }

    for (auto& h : hilos_clientes) h.join();
    logger("TENEDOR", "Todos los clientes desconectados");

    if (servidor_vivo) {
        logger("TENEDOR", "Enviando P/Q/ al servidor para cierre ordenado");
        Mensaje fin;
        fin.id_cliente = -1;
        fin.contenido  = "P/Q/";
        fin.req_id     = "shutdown";
        salida_servidor.push(fin);
    }

    hilo_escucha.join();
    logger("TENEDOR", "Tenedor cerrado");
}
