#include "Cliente.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>
#include <sstream>
#include <atomic>

using namespace std;

extern mutex console_mtx;
extern atomic<int> req_counter;
extern void logger(const string&, const string&);

Cliente::Cliente(int id, Cola& salida, Cola& entrada)
    : id(id), salida(salida), entrada(entrada) {}

void Cliente::enviar(const string& msg) {
    Mensaje m;
    m.id_cliente = id;
    m.contenido = msg;
    // El contador es atómico y global, así que cada req_id es único aunque
    // varios clientes hagan push casi al mismo tiempo
    m.req_id = "req-" + to_string(++req_counter);
    logger("CLIENTE_" + to_string(id), "[" + m.req_id + "] Enviando: " + msg);
    salida.push(m);
}

string Cliente::esperar() {
    Mensaje m = entrada.pop();
    logger("CLIENTE_" + to_string(id), "[" + m.req_id + "] Respuesta: " + m.contenido);
    return m.contenido;
}

void Cliente::correr() {
    logger("CLIENTE_" + to_string(id), "Iniciando sesion con Tenedor");

    // El retraso escalonado evita que todos los clientes golpeen la cola
    // de entrada del Tenedor al mismo tiempo al arrancar
    this_thread::sleep_for(chrono::milliseconds(id * 300));

    enviar("P/R/dir");
    string dir = esperar();

    // P/S/ significa que el Servidor cayó antes de que pudiéramos pedir nada
    if (dir == "P/S/") {
        logger("CLIENTE_" + to_string(id), "P/S/ - Sin servidor disponible, cerrando sesion");
        enviar("P/Q/");
        esperar();
        return;
    }

    // P/D/404 o una respuesta demasiado corta para tener contenido útil
    if (dir == "P/D/404" || dir.size() <= 4) {
        logger("CLIENTE_" + to_string(id), "Sin figuras disponibles");
        enviar("P/Q/");
        esperar();
        return;
    }

    // La respuesta tiene la forma "P/D/figura1,figura2,figura3"
    // Los cuatro primeros caracteres son el prefijo del protocolo
    string figuras_raw = dir.substr(4);
    vector<string> lista;
    stringstream ss(figuras_raw);
    string token;
    while (getline(ss, token, ',')) lista.push_back(token);

    logger("CLIENTE_" + to_string(id), "Figuras disponibles: " + figuras_raw);

    // El módulo garantiza que el índice no se sale del vector aunque haya
    // más clientes que figuras
    string figura = lista[id % lista.size()];
    logger("CLIENTE_" + to_string(id), "Solicitando figura: " + figura);

    this_thread::sleep_for(chrono::milliseconds(100));

    enviar("P/G/" + figura);
    string piezas = esperar();

    if (piezas == "P/S/") {
        logger("CLIENTE_" + to_string(id), "P/S/ - Servidor caido al solicitar figura");
    } else if (piezas == "P/D/404") {
        logger("CLIENTE_" + to_string(id), "P/D/404 - Figura no encontrada");
    } else {
        // Respuesta normal: "P/D/pieza1,pieza2,..."
        logger("CLIENTE_" + to_string(id), "Piezas para " + figura + ": " + piezas.substr(4));
    }

    this_thread::sleep_for(chrono::milliseconds(100));

    enviar("P/Q/");
    esperar();
    logger("CLIENTE_" + to_string(id), "Sesion cerrada");
}
