/**
 * @file Servidor.cc
 * @brief Implementación del servidor de piezas LEGO
 * 
 * Contiene la lógica de búsqueda y gestión del inventario de piezas.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#include "Servidor.h"

ServidorPiezas::ServidorPiezas() {
    inventario["brick_2x4"] = "GvRWcaqB==";
    inventario["placa_2x2"] = "HzSTdbRC==";
    inventario["brick_1x1"] = "IuPQecSD==";
}

void ServidorPiezas::procesar(const string& request, string& response) {
    if (request == "BUSCAR_LISTA") {
        string lista = "OK LISTAR";
        for (auto& par : inventario)
            lista += " " + par.first;
        response = lista;
        return;
    }

    if (request.find("BUSCAR") != 0) {
        response = "ERROR COMANDO_DESCONOCIDO";
        return;
    }

    size_t pos1 = request.find(" ");
    size_t pos2 = request.find_last_of(" ");

    if (pos1 == string::npos || pos1 == pos2) {
        response = "ERROR FORMATO_INVALIDO";
        return;
    }

    string pieza = request.substr(pos1 + 1, pos2 - pos1 - 1);
    string id = request.substr(pos2 + 1);

    if (inventario.count(pieza)) {
        response = "ENTREGAR " + pieza + " " + inventario[pieza] + " " + id;
    } else {
        response = "ERROR " + pieza + " NO_ENCONTRADA " + id;
    }
}