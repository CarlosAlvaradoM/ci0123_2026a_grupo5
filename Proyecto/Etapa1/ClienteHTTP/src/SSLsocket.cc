/**
 * @file SSLsocket.cc
 * @brief Implementacion de la clase SSLsocket para comunicaciones seguras
 * 
 * Proporciona una capa de seguridad SSL/TLS sobre sockets TCP
 * Utiliza la biblioteca OpenSSL para encriptacion
 * 
 * @author Carlos Alvarado && Kiara Brenes
 * @date 2026
 */

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSLsocket.h"

/**
 * @brief Constructor de SSLsocket
 * 
 * Inicializa las bibliotecas OpenSSL, crea el contexto SSL y llama al constructor de Socket
 * 
 * @param type Tipo de socket ('s' para TCP)
 * @param ipv6 True para IPv6, false para IPv4
 */
SSLsocket::SSLsocket(char type, bool ipv6) : Socket(type, ipv6) {
   SSL_library_init();
   OpenSSL_add_all_algorithms();
   SSL_load_error_strings();
   ctx = SSL_CTX_new(TLS_client_method());
   if (!ctx) {
      throw std::runtime_error("SSLsocket: SSL_CTX_new falló");
   }
   ssl = nullptr;
}

/**
 * @brief Destructor de SSLsocket
 * 
 * Cierra la conexion SSL, libera el objeto SSL y el contexto SSL
 */
SSLsocket::~SSLsocket() {
   if (ssl) {
      SSL_shutdown(ssl);
      SSL_free(ssl);
   }
   if (ctx) {
      SSL_CTX_free(ctx);
   }
   EVP_cleanup();
}

/**
 * @brief Establece conexion SSL/TLS usando direccion IP y puerto
 * 
 * @param hostip Direccion IP en formato texto
 * @param port Numero de puerto
 * @return int Retorna 0 si la conexion fue exitosa
 */
int SSLsocket::Connect(const char* hostip, int port) {
   int st = TryToConnect(hostip, port);
   if (st == 0) {
      return SSLConnect();
   }
   return st;
}

/**
 * @brief Establece conexion SSL/TLS usando nombre de host y servicio
 * 
 * @param host Nombre del host
 * @param service Nombre del servicio o puerto
 * @return int Retorna 0 si la conexion fue exitosa
 */
int SSLsocket::Connect(const char* host, const char* service) {
   int st = TryToConnect(host, service);
   if (st == 0) {
      return SSLConnect();
   }
   return st;
}

/**
 * @brief Realiza el handshake SSL/TLS con el servidor
 * 
 * @return int Retorna 0 si el handshake fue exitoso
 * @throw std::runtime_error Si falla la creacion del objeto SSL o el handshake
 */
int SSLsocket::SSLConnect() {
   ssl = SSL_new(ctx);
   if (!ssl) {
      throw std::runtime_error("SSLsocket: SSL_new falló");
   }
   SSL_set_fd(ssl, sockId);
   int st = SSL_connect(ssl);
   if (st != 1) {
      ERR_print_errors_fp(stderr);
      throw std::runtime_error("SSLsocket: SSL_connect falló");
   }
   return 0;
}

/**
 * @brief Lee datos del socket SSL
 * 
 * @param buffer Puntero al buffer donde almacenar los datos
 * @param size Tamano maximo de bytes a leer
 * @return size_t Numero de bytes leidos
 * @throw std::runtime_error Si ocurre un error en SSL_read
 */
size_t SSLsocket::Read(void* buffer, size_t size) {
   if (!ssl) {
      throw std::runtime_error("SSLsocket: Read sin conexión SSL");
   }
   int bytes = SSL_read(ssl, buffer, size);
   if (bytes <= 0) {
      int err = SSL_get_error(ssl, bytes);
      if (err != SSL_ERROR_ZERO_RETURN) {
         throw std::runtime_error("SSLsocket: SSL_read falló");
      }
      return 0;
   }
   return bytes;
}

/**
 * @brief Escribe datos en el socket SSL
 * 
 * @param buffer Puntero a los datos a escribir
 * @param size Numero de bytes a escribir
 * @return size_t Numero de bytes escritos
 * @throw std::runtime_error Si ocurre un error en SSL_write
 */
size_t SSLsocket::Write(const void* buffer, size_t size) {
   if (!ssl) {
      throw std::runtime_error("SSLsocket: Write sin conexión SSL");
   }
   int bytes = SSL_write(ssl, buffer, size);
   if (bytes <= 0) {
      throw std::runtime_error("SSLsocket: SSL_write falló");
   }
   return bytes;
}

/**
 * @brief Escribe una cadena de texto en el socket SSL
 * 
 * @param str Cadena terminada en null a escribir
 * @return size_t Numero de bytes escritos
 */
size_t SSLsocket::Write(const char* str) {
   size_t length = strlen(str);
   return Write(str, length);
}