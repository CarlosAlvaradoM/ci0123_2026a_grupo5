/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2026-i
  *  Grupos: 2 y 3
  *
  ****** VSocket base class implementation
  *
  * (Fedora version)
  *
 **/

#include <sys/socket.h>
#include <arpa/inet.h>		// ntohs, htons
#include <stdexcept>            // runtime_error
#include <cstring>		// memset
#include <netdb.h>		// getaddrinfo, freeaddrinfo
#include <unistd.h>		// close
/*
#include <cstddef>
#include <cstdio>

//#include <sys/types.h>
*/
#include "VSocket.h"


/**
  *  Class creator (constructor)
  *     use Unix socket system call
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
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


/**
  *  Class creator (constructor)
  *     use Unix socket system call
  *
  *  @param     int id: socket identifier
  *
 **/
void VSocket::Init( int id ){

   this->sockId = id;

   if ( this->sockId < 0 ) {
      throw std::runtime_error("VSocket::Init(int)");
   }

}

/**
  * Class destructor
  *
 **/
VSocket::~VSocket() {

   this->Close();

}

/**
  * Class destructor
  *
 **/
void VSocket::Close(){

   if ( this->sockId >= 0 ) {
      if ( close(this->sockId) < 0 ) {
         throw std::runtime_error("VSocket::Close()");
      }
      this->sockId = -1;
   }

}



/**
  * TryToConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.84.166.62"
  * @param      int port: process address, example 80
  *
 **/
int VSocket::TryToConnect( const char * hostip, int port ) {

   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(addr));

   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);

   if ( inet_pton(AF_INET, hostip, &addr.sin_addr) <= 0 ) {
      throw std::runtime_error("VSocket::TryToConnect inet_pton()");
   }

   int st = connect(this->sockId, (struct sockaddr*)&addr, sizeof(addr));

   if ( st < 0 ) {
      throw std::runtime_error("VSocket::TryToConnect connect()");
   }

   return st;
}


/**
  * TryToConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
  * @param      char * service: process address, example "http"
  *
 **/
int VSocket::TryToConnect( const char *host, const char *service ) {

   int port = atoi(service);   // convierte "8080" → 8080

   return this->TryToConnect(host, port);
}


/**
  * Bind method
  *    use "bind" Unix system call (man 3 bind) (server mode)
  *
  * @param      int port: bind a unamed socket to a port defined in sockaddr structure
  *
  *  Links the calling process to a service at port
  *
 **/
int VSocket::Bind( int port ) {

   this->port = port;

   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(addr));

   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = INADDR_ANY;
   addr.sin_port = htons(port);

   int st = bind(this->sockId, (struct sockaddr*)&addr, sizeof(addr));

   if ( st < 0 ) {
      throw std::runtime_error("VSocket::Bind()");
   }

   return st;
}


/**
  * MarkPassive method
  *    use "listen" Unix system call (man listen) (server mode)
  *
  * @param      int backlog: defines the maximum length to which the queue of pending connections for this socket may grow
  *
  *  Establish socket queue length
  *
 **/
int VSocket::MarkPassive( int backlog ) {

   int st = listen(this->sockId, backlog);

   if ( st < 0 ) {
      throw std::runtime_error("VSocket::MarkPassive()");
   }

   return st;
}


/**
  * WaitForConnection method
  *    use "accept" Unix system call (man 3 accept) (server mode)
  *
  *
  *  Waits for a peer connections, return a sockfd of the connecting peer
  *
 **/
int VSocket::WaitForConnection( void ) {

   struct sockaddr_in addr;
   socklen_t len = sizeof(addr);

   int client = accept(this->sockId, (struct sockaddr*)&addr, &len);

   if ( client < 0 ) {
      throw std::runtime_error("VSocket::WaitForConnection()");
   }

   return client;
}

/**
  * Shutdown method
  *    use "shutdown" Unix system call (man 3 shutdown) (server mode)
  *
  *
  *  cause all or part of a full-duplex connection on the socket associated with the file descriptor socket to be shut down
  *
 **/
int VSocket::Shutdown( int mode ) {

   int st = shutdown(this->sockId, mode);

   if ( st < 0 ) {
      throw std::runtime_error("VSocket::Shutdown()");
   }

   return st;
}


// UDP methods 2025

/**
  *  sendTo method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to send data
  *
  *  Send data to another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {
   int st = -1;

   return st;

}


/**
  *  recvFrom method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to receive from data
  *
  *  @return	size_t bytes received
  *
  *  Receive data from another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {
   int st = -1;

   return st;

}
