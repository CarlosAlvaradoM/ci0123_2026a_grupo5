/**
 * @file Tenedor.h
 * @brief Tenedor que actúa como intermediario entre cliente y servidor
 * 
 * Define la clase Tenedor que maneja sesiones de clientes, procesa comandos
 * y se comunica con el servidor de piezas. Implementa el protocolo de
 * comunicación CI-0123.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#pragma once
#include <string>
#include <map>
#include "Servidor.h"

using namespace std;

/**
 * @class Tenedor
 * @brief Intermediario que procesa comandos de clientes y consulta al servidor
 * 
 * El tenedor mantiene un registro de sesiones activas, procesa comandos
 * como CONNECT, DISCONNECT, PING, STATUS, LISTAR, SOLICITAR y GET.
 * Para solicitudes de piezas, genera IDs únicos y se comunica con el
 * servidor backend.
 */
class Tenedor {
private:
    ServidorPiezas servidor;      ///< Servidor backend de piezas
    map<int, bool> sesiones;      ///< Mapa de sesiones activas (id -> estado)

    /**
     * @brief Genera un ID único para cada solicitud
     * @return string con formato "req-XXXX" donde XXXX es un contador incremental
     */
    string generarId();

public:
    /**
     * @brief Procesa un mensaje recibido del cliente
     * @param msg Mensaje del cliente con el comando y parámetros
     * @param resp Referencia donde se almacenará la respuesta
     * 
     * Comandos soportados:
     * - CONNECT [id]: Registra nueva sesión
     * - DISCONNECT [id]: Elimina sesión
     * - PING: Responde con PONG
     * - STATUS: Retorna número de sesiones activas
     * - LISTAR: Lista piezas disponibles en el servidor
     * - SOLICITAR [pieza]: Solicita una pieza, genera reenvío
     * - GET [pieza]: Obtiene pieza directamente
     */
    void procesar(const string& msg, string& resp);
};