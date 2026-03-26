/**
 * @file Tenedor.cc
 * @brief Implementación del tenedor intermediario
 * 
 * Contiene la lógica de procesamiento de comandos, manejo de sesiones
 * y comunicación con el servidor de piezas.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#include "Tenedor.h"
#include <sstream>
#include <mutex>
#include <string>

using namespace std;

extern mutex console_mtx;
extern void logger(const string&, const string&);

/**
 * @brief Genera un identificador único para cada solicitud
 * @return string con formato "req-XXXX" (ej: "req-1000", "req-1001", ...)
 */
string Tenedor::generarId() {
    static int contador = 1000;
    return "req-" + to_string(contador++);
}

void Tenedor::procesar(const string& msg, string& resp) {
    logger("TENEDOR", "Recibido: " + msg);

    if (msg.find("CONNECT") == 0) {
        int id = stoi(msg.substr(8));
        sesiones[id] = true;
        resp = "OK";
    }
    else if (msg.find("DISCONNECT") == 0) {
        int id = stoi(msg.substr(11));
        sesiones.erase(id);
        resp = "OK";
    }
    else if (msg == "PING") {
        resp = "PONG";
    }
    else if (msg == "STATUS") {
        resp = "OK STATUS sesiones_activas=" + to_string(sesiones.size());
    }
    else if (msg == "LISTAR") {
        string resp_servidor;
        servidor.procesar("BUSCAR_LISTA", resp_servidor);
        resp = resp_servidor;
    }
    else if (msg.find("SOLICITAR") == 0) {
        if (msg.size() <= 10) {
            resp = "ERROR FORMATO_INVALIDO";
            return;
        }

        string pieza = msg.substr(10);
        string id_solicitud = generarId();

        logger("TENEDOR", "OK SOLICITAR");
        logger("TENEDOR", "Encolando: " + pieza + " id=" + id_solicitud);

        string msg_servidor = "BUSCAR " + pieza + " " + id_solicitud;
        string resp_servidor;

        logger("HILO_TRABAJADOR", "Enviando: " + msg_servidor);
        servidor.procesar(msg_servidor, resp_servidor);
        logger("HILO_TRABAJADOR", "Recibido: " + resp_servidor);

        if (resp_servidor.find("ENTREGAR") == 0) {
            size_t p1 = resp_servidor.find(" ");
            size_t p2 = resp_servidor.find(" ", p1 + 1);
            size_t p3 = resp_servidor.find(" ", p2 + 1);
            string datos = resp_servidor.substr(p2 + 1, p3 - p2 - 1);
            resp = "REENVIAR " + pieza + " " + datos;
        } else {
            resp = "ERROR " + pieza + " NO_ENCONTRADA";
        }
    }
    else if (msg.find("GET") == 0) {
        if (msg.size() <= 4) {
            resp = "NOTFOUND";
            return;
        }
        string recurso = msg.substr(4);
        string resp_servidor;
        servidor.procesar("BUSCAR " + recurso + " get-req", resp_servidor);
        if (resp_servidor.find("ENTREGAR") == 0) {
            size_t p1 = resp_servidor.find(" ");
            size_t p2 = resp_servidor.find(" ", p1 + 1);
            size_t p3 = resp_servidor.find(" ", p2 + 1);
            string datos = resp_servidor.substr(p2 + 1, p3 - p2 - 1);
            resp = "OK " + datos;
        } else {
            resp = "NOTFOUND";
        }
    }
    else {
        resp = "ERROR COMANDO_DESCONOCIDO";
    }
}