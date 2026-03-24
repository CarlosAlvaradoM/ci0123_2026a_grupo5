/**
 * @file VSocket.h
 * @brief Clase base abstracta para manejo de sockets en Unix/Linux
 */

#ifndef VSocket_h
#define VSocket_h

/**
 * @class VSocket
 * @brief Clase base abstracta para operaciones con sockets
 * 
 * Clase con creacion, conexion y operaciones de E/S sobre sockets
 * 
 * Implemetar los metodos virtuales puros
 */
class VSocket {
   public:
       /**
        * @brief Inicializa un nuevo socket
        * 
        * Crea un socket con la llamada al sistema socket()
        * 
        * @param type Tipo de socket a crear TCP o UDP
        * @param ipv6 True para IPV6 o False para IPV4
        */
       void Init(char type, bool ipv6 = false);
       
       /**
        * @brief Destructor de la clase VSocket
        * 
        * Llama al metodo Close() para cerrar el socket
        */
      ~VSocket();

      /**
       * @brief Cierra la conexión del socket
       * 
       * Cierra el descriptor de socket utilizando la llamada al sistema close()
       */
      void Close();
      
      /**
       * @brief Intenta establecer una conexion usando dirección IP numrica
       * 
       * Conecta el socket a un servidor especificado por dirección IP y puerto
       * 
       * @param hostip Dirección IP ("10.84.166.62")
       * @param port Numero de puerto (8080)
       * @return int Retorna 0 si la conexion fue exitosa
       */
      int TryToConnect(const char* hostip, int port);
      
      /**
       * @brief Intenta establecer una conexion usando nombre de host y servicio
       * 
       * Conecta el socket a un servidor por nombre de dominio y nombre de servicio
       * 
       * @param host Nombre del host ("os.ecci.ucr.ac.cr")
       * @param service Nombre del servicio o numero de puerto ("http", "80", "8080")
       * @return int Retorna 0 si la conexion fue exitosa
       */
      int TryToConnect(const char* host, const char* service);
      
      /**
       * @brief Metodo virtual puro para establecer conexion
       * 
       * @param hostip Dirección IP en decimal con puntos
       * @param port Numero de puerto
       * @return int Estado de la conexion
       */
      virtual int Connect(const char* hostip, int port) = 0;
      
      /**
       * @brief Metodo virtual puro para establecer conexion
       * 
       * @param host Nombre del host
       * @param service Nombre del servicio o puerto
       * @return int Estado de la conexion
       */
      virtual int Connect(const char* host, const char* service) = 0;

      /**
       * @brief Metodo virtual puro para leer datos del socket
       * 
       * @param buffer Puntero al buffer donde se van a guardar los datos leidos
       * @param size Numero maximo de bytes a leer
       * @return size_t Numero de bytes realmente leidos
       */
      virtual size_t Read(void* buffer, size_t size) = 0;
      
      /**
       * @brief Metodo virtual puro para escribir datos en el socket
       * 
       * @param buffer Puntero a los datos a escribir
       * @param size Numero de bytes a escribir
       * @return size_t Numero de bytes realmente escritos
       */
      virtual size_t Write(const void* buffer, size_t size) = 0;
      
      /**
       * @brief Metodo virtual puro para escribir una cadena en el socket
       * 
       * @param str Cadena de caracteres terminada en null a escribir
       * @return size_t Numero de bytes realmente escritos
       */
      virtual size_t Write(const char* str) = 0;

   protected:
      int sockId;   /**< Identificador del descriptor de socket */
      bool IPv6;   /**< Indica si es un socket IPv6 (true) o IPv4 (false) */
      int port;   /**< Puerto asociado al socket */
      char type;   /**< Tipo de socket ('s' para stream, 'd' para datagram) */
        
};
#endif
