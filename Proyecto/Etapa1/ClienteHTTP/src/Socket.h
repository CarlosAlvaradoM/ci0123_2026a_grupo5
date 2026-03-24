/**
 * @file Socket.h
 * @brief Clase derivada para manejo de sockets en Unix/Linux
 */
#ifndef Socket_h
#define Socket_h

#include "VSocket.h"

/**
 * @class Socket
 * @brief Implementacion de un socket para comunicaciones en red
 */
class Socket : public VSocket {

   public:
      /**
       * @brief Constructor de la clase Socket
       * 
       * Crea e inicializa un nuevo socket del tipo especificado.
       * 
       * @param type Tipo de socket a crear TCP o UDP
       * @param ipv6 True IPV6 o False IPV4
       */
      Socket(char type, bool ipv6 = false);
      
      /**
       * @brief Destructor de la clase Socket
       * 
       * Cierra el socket si esta abierto
       */
      ~Socket();
      
      /**
       * @brief Establece una conexion usando direccion IP y puerto
       * 
       * Conecta el socket a un servidor especificado por direccion IP y puerto
       * 
       * @param hostip Direccion IP ("10.1.166.62")
       * @param port Numero de puerto (80, 8080, 443)
       * @return int Retorna 0 si la conexion fue exitosa
       */
      int Connect(const char* hostip, int port);
      
      /**
       * @brief Establece una conexion usando nombre de host y servicio
       * 
       * @param host Nombre del host ("os.ecci.ucr.ac.cr")
       * @param service Nombre del servicio o numero de puerto ("http", "80", "443", "https")
       * @return int Retorna 0 si la conexion fue exitosa
       */
      int Connect(const char* host, const char* service);
      
      /**
       * @brief Lee datos del socket
       * 
       * Lee hasta 'size' bytes del socket y los almacena en el buffer
       * 
       * @param buffer Puntero al buffer donde se van a guardar los datos leidos
       * @param size Numero maximo de bytes a leer
       * @return size_t Numero de bytes realmente leidos
       */
      size_t Read(void* buffer, size_t size);
      
      /**
       * @brief Escribe datos en el socket
       * 
       * Escribe 'size' bytes del buffer en el socket
       * 
       * @param buffer Puntero a los datos a escribir
       * @param size Numero de bytes a escribir
       * @return size_t Numero de bytes realmente escritos
       */
      size_t Write(const void* buffer, size_t size);
      
      /**
       * @brief Escribe una cadena de texto en el socket
       * 
       * @param str Cadena de caracteres terminada en null a escribir
       * @return size_t Numero de bytes realmente escritos
       */
      size_t Write(const char* str);

   protected:
      // No hay atributos protegidos adicionales
};

#endif
