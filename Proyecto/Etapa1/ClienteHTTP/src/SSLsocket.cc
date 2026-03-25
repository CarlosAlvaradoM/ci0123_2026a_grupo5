#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSLsocket.h"

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

int SSLsocket::Connect(const char* hostip, int port) {
   int st = TryToConnect(hostip, port);
   if (st == 0) {
      return SSLConnect();
   }
   return st;
}

int SSLsocket::Connect(const char* host, const char* service) {
   int st = TryToConnect(host, service);
   if (st == 0) {
      return SSLConnect();
   }
   return st;
}

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

size_t SSLsocket::Write(const char* str) {
   size_t length = strlen(str);
   return Write(str, length);
}