#include <sys/socket.h>         // sockaddr_in
#include <arpa/inet.h>          // ntohs
#include <unistd.h>		// write, read
#include <cstring>
#include <stdexcept>
#include <stdio.h>		// printf

#include "Socket.h"		// Derived class

Socket::Socket( char t, bool IPv6 ){

   this->Init( t, IPv6 );      // Call base class constructor

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

   // Usar el metodo read()
   // Parametros: descriptor del socket, buffer, tamano maximo a leer
   ssize_t readBytes = read(this->sockId, buffer, size);

   // Error
   if ( readBytes == -1 ) {
      throw std::runtime_error( "Socket::Read( void *, size_t )" );
   }

   return readBytes;

}

size_t Socket::Write( const void * buffer, size_t size ) {

   // Usar el metodo Write()
   // Parametros: descriptor de socket, buffer, tamano a escribir
   ssize_t writtenBytes = write(this->sockId, buffer, size);

   // Error
   if ( writtenBytes == -1 ) {
      throw std::runtime_error( "Socket::Write( void *, size_t )" );
   }

   return writtenBytes;

}

size_t Socket::Write( const char * text ) {

   // Calcular la longitud del string
   // Usar strlen: cuenta hasta caracter nulo
   size_t lenght = strlen(text);

   // Usar el metodo writte de buffer y tamano
   return this->Write(text, lenght);

}

