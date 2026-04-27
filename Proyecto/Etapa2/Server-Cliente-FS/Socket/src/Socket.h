// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef Socket_h
#define Socket_h
#include "VSocket.h"

/**
  *  Class constructor
  *     use Unix socket system call
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
class Socket : public VSocket {

   public:
      Socket( char, bool = false );
      ~Socket() override;
      Socket( int );

      /**
        * Connect method
        *   use "TryToConnect" in base class
        *
        * @param      char * host: host address in dot notation, example "10.1.166.62"
        * @param      int port: process address, example 80
        *
       **/
      int Connect( const char *, int );

      /**
        * Connect method
        *   use "TryToConnect" in base class
        *
        * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
        * @param      char * service: process address, example "http"
        *
       **/
      int Connect( const char *, const char * );

      /**
        * Read method
        *   use "read" Unix system call (man 3 read)
        *
        * @param      void * buffer: buffer to store data read from socket
        * @param      int size: buffer capacity, read will stop if buffer is full
        *
       **/
      size_t Read( void *, size_t );

      /**
        * Write method
        *   use "write" Unix system call (man 3 write)
        *
        * @param      void * buffer: buffer to store data write to socket
        * @param      size_t size: buffer capacity, number of bytes to write
        *
       **/
      size_t Write( const void *, size_t );

      /**
        * Write method
        *   use "write" Unix system call (man 3 write)
        *
        * @param      char * text: text to write to socket
        *
       **/
      size_t Write( const char * );

      /**
        * AcceptConnection method
        *    use base class to accept connections
        *
        *  @returns   a new class instance
        *
        *  Waits for a new connection to service (TCP mode: stream)
        *
       **/
      VSocket * AcceptConnection();

   protected:

};

#endif