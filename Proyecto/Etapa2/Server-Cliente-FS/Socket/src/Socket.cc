// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <sys/socket.h>         // sockaddr_in
#include <arpa/inet.h>          // ntohs
#include <unistd.h>		// write, read
#include <cstring>
#include <stdexcept>
#include <stdio.h>		// printf

#include "Socket.h"		// Derived class

Socket::Socket( char t, bool IPv6 ){

   this->Init( t, IPv6 );

}


Socket::Socket( int id ) {
   this->Init(id);
}


Socket::~Socket() {

}


int Socket::Connect( const char * hostip, int port ) {

   return this->TryToConnect( hostip, port );

}


int Socket::Connect( const char *host, const char *service ) {

   return this->TryToConnect( host, service );

}


size_t Socket::Read( void * buffer, size_t size ) {

   int st = read(this->sockId, buffer, size);

   if ( st < 0 ) {
      throw std::runtime_error("Socket::Read()");
   }

   return st;
}


size_t Socket::Write( const void * buffer, size_t size ) {

   int st = write(this->sockId, buffer, size);

   if ( st < 0 ) {
      throw std::runtime_error("Socket::Write(void*)");
   }

   return st;
}


size_t Socket::Write( const char * text ) {

   int st = write(this->sockId, text, strlen(text));

   if ( st < 0 ) {
      throw std::runtime_error("Socket::Write(char*)");
   }

   return st;
}


VSocket * Socket::AcceptConnection(){
   int id;
   VSocket * peer;

   id = this->WaitForConnection();

   peer = new Socket( id );

   return peer;

}