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

// Constructor
Cliente::Cliente(int id, Cola& salida, Cola& entrada)
    : id(id), salida(salida), entrada(entrada) {}

// Envía un mensaje al servidor a través de la cola de salida
void Cliente::enviar(const string& msg) {
    Mensaje m;
    m.id_cliente = id;
    m.contenido = msg;
    m.req_id = "req-" + to_string(++req_counter);
    logger("CLIENTE_" + to_string(id), "[" + m.req_id + "] Enviando: " + msg);
    salida.push(m);
}

// Espera a recibir un mensaje de la cola de entrada
string Cliente::esperar() {
    Mensaje m = entrada.pop();
    logger("CLIENTE_" + to_string(id), "[" + m.req_id + "] Respuesta: " + m.contenido);
    return m.contenido;
}

// Ejecuta el flujo principal del cliente
void Cliente::correr() {
    // Registro de inicio de sesión
    logger("CLIENTE_" + to_string(id), "Iniciando sesion con Tenedor");
    // Evitar colisiones
    this_thread::sleep_for(chrono::milliseconds(id * 300));

    // Solicitar dirección inicial
    enviar("P/R/dir");
    string dir = esperar();

    // No hay servidor disponible
    if (dir == "P/S/") {
        logger("CLIENTE_" + to_string(id), "P/S/ - Sin servidor disponible, cerrando sesion");
        enviar("P/Q/");
        esperar();
        return;
    }

    // Error 404 o formato inválido
    if (dir == "P/D/404" || dir.size() <= 4) {
        logger("CLIENTE_" + to_string(id), "Sin figuras disponibles");
        enviar("P/Q/");
        esperar();
        return;
    }

    // Procesar lista de figuras disponibles
    string figuras_raw = dir.substr(4);
    vector<string> lista;
    stringstream ss(figuras_raw);
    string token;
    while (getline(ss, token, ',')) lista.push_back(token);

    logger("CLIENTE_" + to_string(id), "Figuras disponibles: " + figuras_raw);

    // Seleccionar una figura basada en el ID del cliente
    string figura = lista[id % lista.size()];
    logger("CLIENTE_" + to_string(id), "Solicitando figura: " + figura);
    // Pausa para simular
    this_thread::sleep_for(chrono::milliseconds(100));

    // Solicitar una figura específica
    enviar("P/G/" + figura);
    string piezas = esperar();

    // Manejar diferentes respuestas del servidor
    if (piezas == "P/S/") {
        logger("CLIENTE_" + to_string(id), "P/S/ - Servidor caido al solicitar figura");
    } else if (piezas == "P/D/404") {
        logger("CLIENTE_" + to_string(id), "P/D/404 - Figura no encontrada");
    } else {
        logger("CLIENTE_" + to_string(id), "Piezas para " + figura + ": " + piezas.substr(4));
    }

    // Pausa para simular
    this_thread::sleep_for(chrono::milliseconds(100));

    // Cerrar sesión
    enviar("P/Q/");
    esperar();
    logger("CLIENTE_" + to_string(id), "Sesion cerrada");
}
