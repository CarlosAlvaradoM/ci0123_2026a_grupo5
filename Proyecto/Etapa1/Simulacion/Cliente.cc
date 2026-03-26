/**
 * @file Cliente.cc
 * @brief Implementación de la clase Cliente
 * 
 * Contiene la lógica de la secuencia de comandos que el cliente ejecuta
 * contra el tenedor para probar el protocolo.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#include "Cliente.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <string>

using namespace std;

extern mutex console_mtx;
extern void logger(const string&, const string&);

/**
 * @brief Ejecuta la secuencia completa de comandos del cliente
 * 
 * La secuencia incluye:
 * - CONNECT: Establece sesión con el tenedor
 * - PING: Verifica conectividad
 * - STATUS: Consulta estado del sistema
 * - LISTAR: Obtiene lista de piezas disponibles
 * - SOLICITAR brick_2x4: Prueba solicitud exitosa con reenvío
 * - SOLICITAR pieza_rara: Prueba solicitud de pieza inexistente
 * - GET brick_1x1: Prueba obtención directa de pieza
 * - COMANDO_FALSO: Prueba manejo de comandos inválidos
 * - DISCONNECT: Finaliza la sesión
 */
void Cliente::ejecutar() {
    string resp;
    string sid = to_string(id);

    logger("CLIENTE", "Enviando: CONNECT " + sid);
    tenedor.procesar("CONNECT " + sid, resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    logger("CLIENTE", "Enviando: PING");
    tenedor.procesar("PING", resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    logger("CLIENTE", "Enviando: STATUS");
    tenedor.procesar("STATUS", resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    logger("CLIENTE", "Enviando: LISTAR");
    tenedor.procesar("LISTAR", resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    string pieza = "brick_2x4";
    logger("CLIENTE", "Enviando: SOLICITAR " + pieza);
    tenedor.procesar("SOLICITAR " + pieza, resp);
    if (resp.find("REENVIAR") == 0) {
        logger("CLIENTE", "Tenedor respondio: " + resp);
        logger("CLIENTE", "Enviando: OK REENVIAR " + pieza);
    } else {
        logger("CLIENTE", "Tenedor respondio: " + resp);
    }

    this_thread::sleep_for(chrono::milliseconds(300));

    string pieza_mala = "pieza_rara";
    logger("CLIENTE", "Enviando: SOLICITAR " + pieza_mala);
    tenedor.procesar("SOLICITAR " + pieza_mala, resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    logger("CLIENTE", "Enviando: GET brick_1x1");
    tenedor.procesar("GET brick_1x1", resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    logger("CLIENTE", "Enviando: COMANDO_FALSO");
    tenedor.procesar("COMANDO_FALSO", resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);

    this_thread::sleep_for(chrono::milliseconds(300));

    logger("CLIENTE", "Enviando: DISCONNECT " + sid);
    tenedor.procesar("DISCONNECT " + sid, resp);
    logger("CLIENTE", "Tenedor respondio: " + resp);
}