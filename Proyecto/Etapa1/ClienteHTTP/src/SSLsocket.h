/**
 * @file SSLsocket.h
 * @brief Clase derivada para manejo de sockets seguros con SSL/TLS
 * 
 * Esta clase extiende la funcionalidad de Socket agregando una capa de
 * seguridad mediante el protocolo SSL/TLS utilizando la biblioteca OpenSSL.
 * Permite establecer conexiones cifradas con servidores HTTPS.
 * 
 * @author Carlos Alvarado && Kiara Brenes
 * @date 2026
 */

#ifndef SSLsocket_h
#define SSLsocket_h

#include "Socket.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

/**
 * @class SSLsocket
 * @brief Implementacion de un socket seguro con SSL/TLS
 * 
 * Proporciona metodos para establecer conexiones seguras y realizar
 * operaciones de lectura/escritura cifradas sobre la capa SSL/TLS.
 * Hereda de Socket y utiliza OpenSSL para el manejo de la seguridad.
 */
class SSLsocket : public Socket {

   public:
      /**
       * @brief Constructor de la clase SSLsocket
       * 
       * Inicializa las bibliotecas OpenSSL y crea el contexto SSL.
       * 
       * @param type Tipo de socket ('s' para TCP o 'd' para UDP)
       * @param ipv6 True para IPv6, false para IPv4 (por defecto false)
       * @throw std::runtime_error Si falla la creacion del contexto SSL
       */
      SSLsocket(char type, bool ipv6 = false);
      
      /**
       * @brief Destructor de la clase SSLsocket
       * 
       * Cierra la conexion SSL, libera el objeto SSL y el contexto SSL
       */
      ~SSLsocket();
      
      /**
       * @brief Establece una conexion segura usando direccion IP y puerto
       * 
       * Primero establece la conexion TCP mediante TryToConnect() y luego
       * realiza el handshake SSL/TLS con el servidor.
       * 
       * @param hostip Direccion IP ("10.84.166.62" o "fe80::1%eno1")
       * @param port Numero de puerto (443, 8443, etc.)
       * @return int Retorna 0 si la conexion fue exitosa
       * @throw std::runtime_error Si falla la conexion TCP o el handshake SSL
       */
      int Connect(const char* hostip, int port);
      
      /**
       * @brief Establece una conexion segura usando nombre de host y servicio
       * 
       * @param host Nombre del host ("os.ecci.ucr.ac.cr")
       * @param service Nombre del servicio o numero de puerto ("https", "443")
       * @return int Retorna 0 si la conexion fue exitosa
       * @throw std::runtime_error Si falla la resolucion DNS o el handshake SSL
       */
      int Connect(const char* host, const char* service);
      
      /**
       * @brief Lee datos del socket seguro
       * 
       * Lee hasta 'size' bytes del canal SSL y los almacena en el buffer.
       * 
       * @param buffer Puntero al buffer donde se van a guardar los datos leidos
       * @param size Numero maximo de bytes a leer
       * @return size_t Numero de bytes realmente leidos
       * @throw std::runtime_error Si ocurre un error durante la lectura SSL
       */
      size_t Read(void* buffer, size_t size);
      
      /**
       * @brief Escribe datos en el socket seguro
       * 
       * Escribe 'size' bytes del buffer en el canal SSL.
       * 
       * @param buffer Puntero a los datos a escribir
       * @param size Numero de bytes a escribir
       * @return size_t Numero de bytes realmente escritos
       * @throw std::runtime_error Si ocurre un error durante la escritura SSL
       */
      size_t Write(const void* buffer, size_t size);
      
      /**
       * @brief Escribe una cadena de texto en el socket seguro
       * 
       * @param str Cadena de caracteres terminada en null a escribir
       * @return size_t Numero de bytes realmente escritos
       * @throw std::runtime_error Si ocurre un error durante la escritura SSL
       */
      size_t Write(const char* str);

   private:
      SSL_CTX* ctx;   /**< Contexto SSL que contiene la configuracion y metodos SSL */
      SSL* ssl;       /**< Objeto SSL que maneja la conexion segura actual */
      
      /**
       * @brief Realiza el handshake SSL/TLS con el servidor
       * 
       * Configura el objeto SSL con el descriptor de socket y ejecuta
       * el protocolo de handshake para establecer la conexion segura.
       * 
       * @return int Retorna 0 si el handshake fue exitoso
       * @throw std::runtime_error Si falla la creacion del objeto SSL o el handshake
       */
      int SSLConnect();
};

#endif