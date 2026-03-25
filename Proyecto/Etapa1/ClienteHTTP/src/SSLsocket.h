/**
 * @file SSLsocket.h
 * @brief Clase derivada para manejo de sockets seguros con SSL/TLS
 */
#ifndef SSLsocket_h
#define SSLsocket_h

#include "Socket.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

/**
 * @class SSLsocket
 * @brief Implementacion de un socket seguro con SSL/TLS
 */
class SSLsocket : public Socket {

   public:
      SSLsocket(char type, bool ipv6 = false);
      ~SSLsocket();
      
      int Connect(const char* hostip, int port);
      int Connect(const char* host, const char* service);
      
      size_t Read(void* buffer, size_t size);
      size_t Write(const void* buffer, size_t size);
      size_t Write(const char* str);

   private:
      SSL_CTX* ctx;
      SSL* ssl;
      int SSLConnect();
};
#endif