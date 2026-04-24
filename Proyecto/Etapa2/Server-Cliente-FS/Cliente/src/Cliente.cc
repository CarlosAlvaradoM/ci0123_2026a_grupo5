// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <sstream>

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
  try {
    this->datosConexion();
  }
  catch(const std::exception& e) {
    fprintf(stderr, "Error tratando de conectar con servidor: %s\n", e.what());
    return;
  }
  
  this->listarFiguras();
  while (true) {
    int eleccion = this->elegirFigura();
    if (eleccion == (int) this->listaFiguras.size()) break;
    this->pedirFigura(this->listaFiguras[eleccion]);
  }
  this->listaFiguras.clear();
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
  std::string request = this->formarRequest(figura);
  this->socket->Write(request.c_str(), request.size() + 1);
  std::string response;
  char buffer[512];
  int n;

  while ((n = this->socket->Read(buffer, sizeof(buffer))) > 0) {
    response.append(buffer, n);
  }
  if (response.empty()) return -1;
  printf("Piezas para armar %s:\n", figura.c_str());
  printf("%s\n", response.c_str());
  return 0;
}

int Cliente::elegirFigura() {
  int count = 0;
  int eleccion = 0;
  printf("Que figura desea consultar (ingrese el numero):\n");
  for (const std::string& figura : this->listaFiguras) {
    printf("%d. %s\n", ++count, figura.c_str());
  }
  printf("%d. Salir\n", ++count);
  while (true) {
    if (scanf("%d", &eleccion) <= 0
        || eleccion < 1 || eleccion > (int) this->listaFiguras.size()) {
      fprintf(stderr, "Opcion invalida, ingrese una de la lista");
      continue;
    }
    break;
  }
  return eleccion - 1;
}

void Cliente::listarFiguras() {
  std::string request = this->formarRequest("listdata");
  this->socket->Write(request.c_str(), request.size() + 1);
  std::stringstream response;
  char buffer[512];
  int n;

  while ((n = this->socket->Read(buffer, sizeof(buffer))) > 0) {
    response.write(buffer, n);
  }

  response.seekg(0);

  std::string line;
  while (std::getline(response, line, '\n')) {
    if (line.empty()) continue;
    this->listaFiguras.push_back(line);
  }
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
  int protocolo = 0;
  int ssl = 0;
  // Consultamos con que protocolo se va a conectar
  while (true) {
    printf("Que protocolo desea usar? (ingrese el numero):\n");
    printf("1. IPv4:\n");
    printf("2. IPv6:\n");
    if (scanf("%d", &protocolo) <= 0 || protocolo < 1 || protocolo > 2) {
      fprintf(stderr, "Opcion invalida, ingrese una de la lista");
      continue;
    }
    break;
  }
  // Lo mismo para saber si usa SSL o no
  while (true) {
    printf("Desea usar SSL para conectarse? (ingrese el numero):\n");
    printf("1. Si:\n");
    printf("2. No:\n");
    if (scanf("%d", &ssl) <= 0 || ssl < 1 || ssl > 2) {
      fprintf(stderr, "Opcion invalida, ingrese una de la lista");
      continue;
    }
    break;
  }
  bool IPv6 = (protocolo == 2);
  if (ssl == 1) {
    // TODO: añadir ssl cuando este disponible
  } else {
    this->socket = new Socket('s', IPv6);
  }

  this->socket->Connect(this->ServerIP.c_str(), this->port);
}
