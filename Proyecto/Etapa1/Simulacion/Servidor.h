/**
 * @file Servidor.h
 * @brief Servidor de piezas LEGO con inventario simulado
 * 
 * Define la clase ServidorPiezas que actúa como backend del sistema,
 * manteniendo un inventario de piezas con sus códigos codificados en Base64.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#pragma once
#include <string>
#include <map>

using namespace std;

/**
 * @class ServidorPiezas
 * @brief Servidor que maneja el inventario de piezas LEGO
 * 
 * Simula un servidor de base de datos que almacena piezas LEGO
 * y sus códigos de fabricación codificados. Responde a comandos
 * como BUSCAR_LISTA y BUSCAR [pieza] [id_solicitud].
 */
class ServidorPiezas {
private:
    map<string, string> inventario;  ///< Mapa de pieza -> código codificado

public:
    /**
     * @brief Constructor que inicializa el inventario con piezas de ejemplo
     * 
     * Inicializa el inventario con tres piezas predefinidas:
     * - brick_2x4: "GvRWcaqB=="
     * - placa_2x2: "HzSTdbRC=="
     * - brick_1x1: "IuPQecSD=="
     */
    ServidorPiezas();
    
    /**
     * @brief Procesa las solicitudes del tenedor
     * @param request Comando a procesar (BUSCAR_LISTA o BUSCAR [pieza] [id])
     * @param response Respuesta generada por el servidor
     * 
     * Para BUSCAR_LISTA retorna "OK LISTAR [lista de piezas]"
     * Para BUSCAR retorna "ENTREGAR [pieza] [codigo] [id]" si la pieza existe,
     * o "ERROR [pieza] NO_ENCONTRADA [id]" en caso contrario.
     */
    void procesar(const string& request, string& response);
};