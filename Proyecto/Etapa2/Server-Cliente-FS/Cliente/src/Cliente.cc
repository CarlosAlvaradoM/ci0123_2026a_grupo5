// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <cctype>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "Cliente.h"
#include "Socket.h"
#include "SSLSocket.h"

#define LIMPIAR_INPUT() \
  int c; \
  while ((c = getchar()) != '\n' && c != EOF)

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

  printf("Conexión exitosa con el servidor %s\n", this->ServerIP.c_str());
  try {
    /* code */
    this->listarFiguras();
    if (this->listaFiguras.empty()) {
      fprintf(stderr, "Error, el servidor no tiene ninguna figura\n");
    } else {
      while (true) {
        int eleccion = this->elegirFigura();
        if (eleccion == (int) this->listaFiguras.size()) break;
        if (this->pedirFigura(this->listaFiguras[eleccion]) == -1) {
          fprintf(stderr, "Error, el servidor ya no contiene dicha figura\n");
        }
      }
    }
  }
  catch(const std::exception& e) {
    fprintf(stderr, "Error tratando de comunicarse con el servidor: %s\n", e.what());
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
  char c;

  while (this->socket->Read(&c, 1) > 0) {
    if (c == '\0') break; 
    response += c;
  }
  if (response.empty()) return -1;
  size_t bodyPos = response.find("\r\n\r\n");
  if (bodyPos == std::string::npos) {
    // No hay estructura HTTP, Se asume que el body llega desde el inicio de la
    // respuesta
    bodyPos = 0;
  } else bodyPos += 4;
  printf("Piezas para armar %s:\n", figura.c_str());
  printf("%s\n", response.c_str() + bodyPos);
  return 0;
}

int Cliente::elegirFigura() {
  int count = 0;
  int eleccion = 0;
  printf("Que figura desea consultar (ingrese el numero):\n");
  for (const std::string& figura : this->listaFiguras) {
    printf("%d. %s\n", ++count, figura.c_str());
  }
  printf("\e[0;31m%d. Salir del programa\e[0m\n", ++count);
  while (true) {
    if (scanf("%d", &eleccion) <= 0
        || eleccion < 1 || eleccion > (int) this->listaFiguras.size() + 1) {
      fprintf(stderr, "Opcion invalida, ingrese una de la lista\n");
      LIMPIAR_INPUT();
      continue;
    }
    break;
  }
  return eleccion - 1;
}

void Cliente::listarFiguras() {
  std::string request = this->formarRequest("");
  this->socket->Write(request.c_str(), request.size() + 1);
  std::string response;
  char c;

  while (this->socket->Read(&c, 1) > 0) {
    if (c == '\0') break; 
    response += c;
  }

  std::stringstream ss(response);

  size_t bodyPos = response.find("\r\n\r\n");
  if (bodyPos == std::string::npos) {
    bodyPos = 0;
  } else {
    bodyPos += 4;
  }
  size_t comaPos = 0;
  while ((comaPos = response.find('\n', bodyPos)) != std::string::npos) {
    std::string figure = response.substr(bodyPos, comaPos - bodyPos);
    this->listaFiguras.push_back(figure);
    bodyPos = comaPos + 1;
  }
}

/**
 * @brief Metodo privado para armar el mensaje del request
 * @param figura lo que se quiere solicitar
 */
std::string Cliente::formarRequest(const std::string& figura) {
  return "GET /" + figura  + " HTTP/1.1\r\n\r\n";
}

void Cliente::datosConexion() {
  int protocolo = 0;
  int ssl = 0;
  // Consultamos con que protocolo se va a conectar
  while (true) {
    printf("Que protocolo desea usar? (ingrese el numero):\n");
    printf("1. IPv4\n");
    printf("2. IPv6\n");
    if (scanf("%d", &protocolo) <= 0 || protocolo > 2) {
      fprintf(stderr, "Opcion invalida, ingrese una de la lista\n");
      // En caso de que quede algun caracter invalido, se limpia la entrada estandar
      LIMPIAR_INPUT();
      continue;
    }
    break;
  }
  // Lo mismo para saber si usa SSL o no
  while (true) {
    printf("Desea usar SSL para conectarse? (ingrese el numero):\n");
    printf("1. Si\n");
    printf("2. No\n");
    if (scanf("%d", &ssl) <= 0 || ssl > 2) {
      fprintf(stderr, "Opcion invalida, ingrese una de la lista\n");
      LIMPIAR_INPUT();
      continue;
    }
    break;
  }
  bool IPv6 = (protocolo == 2);
  if (ssl == 1) {
    this->socket = new SSLSocket(IPv6);
  } else {
    this->socket = new Socket('s', IPv6);
  }

  this->socket->Connect(this->ServerIP.c_str(), this->port);
}
