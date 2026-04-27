// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef VSocket_h
#define VSocket_h

#include <cstddef>

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
class VSocket {
   public:
       void Init( char, bool = false );

       /**
         *  Class creator (constructor)
         *     use Unix socket system call
         *
         *  @param     int id: socket identifier
         *
        **/
       void Init( int );
      virtual ~VSocket() = 0;

      /**
        * Class destructor
        *
       **/
      void Close();

      /**
        * TryToConnect method
        *   use "connect" Unix system call
        *
        * @param      char * host: host address in dot notation, example "10.84.166.62"
        * @param      int port: process address, example 80
        *
       **/
      int TryToConnect( const char *, int );

      /**
        * TryToConnect method
        *   use "connect" Unix system call
        *
        * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
        * @param      char * service: process address, example "http"
        *
       **/
      int TryToConnect( const char *, const char * );
      virtual int Connect( const char *, int ) = 0;
      virtual int Connect( const char *, const char * ) = 0;

      virtual size_t Read( void *, size_t ) = 0;
      virtual size_t Write( const void *, size_t ) = 0;
      virtual size_t Write( const char * ) = 0;

      /**
        * Bind method
        *    use "bind" Unix system call (man 3 bind) (server mode)
        *
        * @param      int port: bind a unamed socket to a port defined in sockaddr structure
        *
        *  Links the calling process to a service at port
        *
       **/
      int Bind( int );

      /**
        * MarkPassive method
        *    use "listen" Unix system call (man listen) (server mode)
        *
        * @param      int backlog: defines the maximum length to which the queue of pending connections for this socket may grow
        *
        *  Establish socket queue length
        *
       **/
      int MarkPassive( int );

      /**
        * WaitForConnection method
        *    use "accept" Unix system call (man 3 accept) (server mode)
        *
        *
        *  Waits for a peer connections, return a sockfd of the connecting peer
        *
       **/
      int WaitForConnection( void );
      virtual VSocket * AcceptConnection() = 0;

      /**
        * Shutdown method
        *    use "shutdown" Unix system call (man 3 shutdown) (server mode)
        *
        *
        *  cause all or part of a full-duplex connection on the socket associated with the file descriptor socket to be shut down
        *
       **/
      int Shutdown( int );

// UDP methods
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
      size_t sendTo( const void *, size_t, void * );

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
      size_t recvFrom( void *, size_t, void * );

   protected:
      int sockId;   // Socket identifier
      bool IPv6;      // Is IPv6 socket?
      int port;       // Socket associated port
      char type;      // Socket type (datagram, stream, etc.)

};

#endif // VSocket_h