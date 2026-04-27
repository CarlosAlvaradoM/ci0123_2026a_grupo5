// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <sys/socket.h>
#include <arpa/inet.h>		// ntohs, htons
#include <stdexcept>            // runtime_error
#include <cstring>		// memset
#include <netdb.h>		// getaddrinfo, freeaddrinfo
#include <unistd.h>		// close

#include "VSocket.h"


void VSocket::Init( char t, bool IPv6 ){

   this->type = t;
   this->IPv6 = IPv6;

   int domain = IPv6 ? AF_INET6 : AF_INET;
   int socktype = (t == 's') ? SOCK_STREAM : SOCK_DGRAM;

   this->sockId = socket(domain, socktype, 0);

   if ( this->sockId < 0 ) {
      throw std::runtime_error("VSocket::Init socket()");
   }

}


void VSocket::Init( int id ){

   this->sockId = id;

   if ( this->sockId < 0 ) {
      throw std::runtime_error("VSocket::Init(int)");
   }

}


VSocket::~VSocket() {

   this->Close();

}


void VSocket::Close(){

   if ( this->sockId >= 0 ) {
      if ( close(this->sockId) < 0 ) {
         throw std::runtime_error("VSocket::Close()");
      }
      this->sockId = -1;
   }

}


int VSocket::TryToConnect(const char *hostip, int port) {

   if (this->IPv6) {
      struct sockaddr_in6 addr6;
      memset(&addr6, 0, sizeof(addr6));

      addr6.sin6_family = AF_INET6;
      addr6.sin6_port = htons(port);

      if (inet_pton(AF_INET6, hostip, &addr6.sin6_addr) <= 0) {
         throw std::runtime_error("IPv6 inet_pton()");
      }

      return connect(this->sockId, (struct sockaddr*)&addr6, sizeof(addr6));
   } else {
      struct sockaddr_in addr;
      memset(&addr, 0, sizeof(addr));

      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);

      if (inet_pton(AF_INET, hostip, &addr.sin_addr) <= 0) {
         throw std::runtime_error("IPv4 inet_pton()");
      }

      return connect(this->sockId, (struct sockaddr*)&addr, sizeof(addr));
   }
}


int VSocket::TryToConnect( const char *host, const char *service ) {

   int port = atoi(service);

   return this->TryToConnect(host, port);
}


int VSocket::Bind(int port) {

   this->port = port;

   if (this->IPv6) {
      struct sockaddr_in6 addr6;
      memset(&addr6, 0, sizeof(addr6));

      addr6.sin6_family = AF_INET6;
      addr6.sin6_addr = in6addr_any;
      addr6.sin6_port = htons(port);

      return bind(this->sockId, (struct sockaddr*)&addr6, sizeof(addr6));
   } else {
      struct sockaddr_in addr;
      memset(&addr, 0, sizeof(addr));

      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = INADDR_ANY;
      addr.sin_port = htons(port);

      return bind(this->sockId, (struct sockaddr*)&addr, sizeof(addr));
   }
}


int VSocket::MarkPassive( int backlog ) {

   int st = listen(this->sockId, backlog);

   if ( st < 0 ) {
      throw std::runtime_error("VSocket::MarkPassive()");
   }

   return st;
}


int VSocket::WaitForConnection( void ) {

   struct sockaddr_in addr;
   socklen_t len = sizeof(addr);

   int client = accept(this->sockId, (struct sockaddr*)&addr, &len);

   if ( client < 0 ) {
      throw std::runtime_error("VSocket::WaitForConnection()");
   }

   return client;
}


int VSocket::Shutdown( int mode ) {

   int st = shutdown(this->sockId, mode);

   if ( st < 0 ) {
      throw std::runtime_error("VSocket::Shutdown()");
   }

   return st;
}


size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {
   int st = -1;

   return st;

}


size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {
   int st = -1;

   return st;

}