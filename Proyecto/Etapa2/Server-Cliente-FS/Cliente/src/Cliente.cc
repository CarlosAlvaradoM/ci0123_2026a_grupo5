// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
#include <stdexcept>

#include "Cliente.h"
#include "Socket.h"

/**
 * @brief Constructor por argumentos
 */
Cliente::Cliente(const std::string& ServerIP, const int port)
  : ServerIP(ServerIP)
  , port(port) {
}

void Cliente::run() {
  this->datosConexion();
  std::vector<std::string> figuras = this->listarFiguras();
  while (true) {
    this->pedirFigura("");
    break;
  }
  delete this->socket;
  this->socket = NULL;
}

/**
 * @brief Rutina para pedir las piezas para armar una figura e imprimir
 * la lista en la salida.
 * @param figura Nombre del archivo a pedir
 * @returns Un código de error. 0 en caso de éxito, -1 al contrario;
 */
int Cliente::pedirFigura(const std::string& figura) {
  
  return 0;
}

std::vector<std::string> Cliente::listarFiguras() {
  return std::vector<std::string>();
}

/**
 * @brief Metodo privado para armar el mensaje del request
 * @param figura lo que se quiere solicitar
 */
std::string Cliente::formarRequest(const std::string& figura) {
  return "GET " 
        + this->ServerIP + ":" + std::to_string(this->port) + "/" + figura 
        + " HTTP/1.1\r\n\r\n";
}

void Cliente::datosConexion() {
}
