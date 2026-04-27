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
  /**
 * @brief Constructor por argumentos
 * @param serverIP IP del servidor a conectarse
 * @param port Puerto por el cual se va a comunicar
 */
  Cliente(const std::string& serverIP, const int port);
  ~Cliente() = default;

  /**
   * @brief Subrutina principal que inicializa el programa.
   * 
   * Pide los datos necesarios para la conexion con el servidor (de direccion 
   * indicada en el constructor) e inicializa el bucle para pedir figuras
   * disponibles
   */
  void run();
  
  private:
  /// Dirección del servidor con el cual el cliente se estará comunicando.
  std::string ServerIP = "";
  /// Puerto donde el server esta escuchando
  int port = 0;
  /// Socket para comunicarse con el server
  VSocket* socket;
  /// Lista de las figuras que contiene el server al momento de la conexión
  std::vector<std::string> listaFiguras;

  /**
 * @brief Subrutina para pedir las piezas para armar una figura e imprimir
 * la lista en la salida.
 * @param figura Nombre del archivo a pedir
 * @returns Un código de error. 0 en caso de éxito, -1 al contrario;
 */
  int pedirFigura(const std::string& figura);

  /**
   * @brief Subrutina para el menu de elegir una figura de las que se consulto
   * @returns la opcion que se escogio del menu, que seria el indice en
   * la lista de figuras
   */
  int elegirFigura();

  /**
   * @brief Subrutina para pedirle al servidor la lista de figuras y añadirlas
   * al vector de strings para poder accederlas facilmente
   */
  void listarFiguras();

  /**
 * @brief Metodo privado para armar el mensaje del request
 * @param figura lo que se quiere solicitar
 */
  std::string formarRequest(const std::string& figura);

  /**
   * @brief Subrutina para el menu de elegir la configuracion de conexion con el
   * servidor
   * 
   * Permite elegir si se conecta por IPv4 o IPv6
   * y si quiere que los datos esten encriptados o no (SSL).
   */
  void datosConexion();
};

