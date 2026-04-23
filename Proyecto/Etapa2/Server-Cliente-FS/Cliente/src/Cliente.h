// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <string>

/**
 * @class Cliente
 * @brief Clase para realizar solicitudes mediante el protocolo HTTPS a un
 * servidor concreto.
 */
class Cliente {
 public:
  Cliente() = delete;
  explicit Cliente(const std::string&);
  ~Cliente() = default;

  int pedirFigura();

 private:
  /// Dirección del servidor con el cual el cliente se estará comunicando.
  std::string dir;

  std::string pedirPagina(const std::string&, int = 1); 

  std::string formarRequest(const std::string&, int);
};

