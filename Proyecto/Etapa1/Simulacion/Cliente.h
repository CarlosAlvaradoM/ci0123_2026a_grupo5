/**
 * @file Cliente.h
 * @brief Cliente que simula interacciones con el tenedor de piezas LEGO
 * 
 * Esta clase representa un cliente que envía comandos al tenedor para
 * probar el protocolo de comunicación. Realiza una secuencia predefinida
 * de operaciones como CONNECT, PING, STATUS, LISTAR, y solicitudes de piezas.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#pragma once
#include "Tenedor.h"

/**
 * @class Cliente
 * @brief Simula un cliente que interactúa con el sistema de piezas LEGO
 * 
 * El cliente ejecuta una serie de comandos contra el tenedor para verificar
 * el correcto funcionamiento del protocolo. Cada cliente tiene un identificador
 * único que lo distingue en el sistema.
 */
class Cliente {
private:
    Tenedor& tenedor;   ///< Referencia al tenedor que maneja las solicitudes
    int id;              ///< Identificador único del cliente

public:
    /**
     * @brief Constructor del cliente
     * @param t Referencia al tenedor que procesará las solicitudes
     * @param id Identificador numérico del cliente
     */
    Cliente(Tenedor& t, int id) : tenedor(t), id(id) {}
    
    /**
     * @brief Ejecuta la secuencia completa de comandos del cliente
     * 
     * Realiza una secuencia de operaciones: CONNECT, PING, STATUS, LISTAR,
     * solicitudes de piezas válidas e inválidas, GET y DISCONNECT. 
     */
    void ejecutar();
};