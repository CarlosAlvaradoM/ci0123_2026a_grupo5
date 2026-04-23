// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include "Socket.h"

#include <string>
#include <vector>

/**
 * @class Cliente
 * @brief Clase para realizar solicitudes mediante el protocolo HTTPS a un
 * servidor concreto.
 */
class Cliente {
 public:
  Cliente() = delete;
  explicit Cliente(const std::string&, const int);
  ~Cliente() = default;

  void run();
  
  private:
  /// Dirección del servidor con el cual el cliente se estará comunicando.
  std::string ServerIP = "";
  /// Puerto donde el server esta escuchando
  int port = 0;
  /// Socket para comunicarse con el server
  VSocket* socket;

  int pedirFigura(const std::string&);
  
  std::vector<std::string> listarFiguras();

  std::string formarRequest(const std::string&);

  void datosConexion();
};

