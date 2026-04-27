/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2026-i
  *  Grupos: 2 y 3
  *
  *  SSL Socket class implementation
  *
  * (Fedora version)
  *
 **/
 
// SSL includes
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdexcept>

#include "SSLSocket.h"
#include "Socket.h"

/**
  *  Class constructor
  *     use base class
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
SSLSocket::SSLSocket( bool IPv6 ) {

   this->Init( 's', IPv6 );

   this->Context = nullptr;
   this->BIO = nullptr;

   this->InitSSL();					// Initializes to client context

}


/**
  *  Class constructor
  *
  *  @param     int id: socket descriptor
  *
 **/
SSLSocket::SSLSocket( int id ) {

   this->Init( id );

}

/**
 *  SSLSocket
 *     use SSL_new with a defined context
 *
 *  Create a SSL object for server conections
 *
 *  @param	const char * certFileName, file containing certificate
 *  @param	const char * keyFileName, file containing keys
 *
 **/
SSLSocket::SSLSocket( const char * certFileName, const char * keyFileNamei, bool IPv6 ) {

   this->Init('s', IPv6);
  
   this->InitSSL(true);

   this->LoadCertificates(certFileName, keyFileNamei);
}


/**
  * Class destructor
  *
 **/
SSLSocket::~SSLSocket() {

// SSL destroy
   if ( nullptr != this->Context ) {
      SSL_CTX_free( reinterpret_cast<SSL_CTX *>( this->Context ) );
   }
   if ( nullptr != this->BIO ) {
      SSL_free( reinterpret_cast<SSL *>( this->BIO ) );
   }

   this->Close();

}


/**
  *  InitSSL
  *     use SSL_new with a defined context
  *
  *  Create a SSL object
  *
 **/
void SSLSocket::InitSSL( bool serverContext ) {
   SSL * ssl = nullptr;

   this->InitContext( serverContext );

   if (this->BIO == nullptr) {
      ssl = SSL_new( (SSL_CTX *) this->Context );
      
      if (ssl == nullptr) {
         throw std::runtime_error( "SSLSocket::InitSSL( bool )" );
      }
   
      this->BIO = (void *) ssl;
   }

}


/**
  *  InitContext
  *     use SSL_library_init, OpenSSL_add_all_algorithms, SSL_load_error_strings, TLS_server_method, SSL_CTX_new
  *
  *  Creates a new SSL server context to start encrypted comunications, this context is stored in class instance
  *
 **/
void SSLSocket::InitContext( bool serverContext ) {
   const SSL_METHOD * method = (serverContext)
      ? TLS_server_method() : TLS_client_method();
   
   if ( nullptr == method ) {
      throw std::runtime_error( "SSLSocket::InitContext( bool ): method" );
   }
   
   if (this->Context == nullptr) {
      SSL_CTX * context;
      context = SSL_CTX_new( method );
   
      if ( nullptr == context ) {
         throw std::runtime_error( "SSLSocket::InitContext( bool ): context" );
      }
   
      this->Context = (void *) context;
   }

}


/**
 *  Load certificates
 *    verify and load certificates
 *
 *  @param	const char * certFileName, file containing certificate
 *  @param	const char * keyFileName, file containing keys
 *
 **/
 void SSLSocket::LoadCertificates( const char * certFileName, const char * keyFileName ) {
   int st = -1;

   if ( SSL_CTX_use_certificate_file( (SSL_CTX *) this->Context, certFileName, SSL_FILETYPE_PEM ) <= 0 ) {	 // set the local certificate from CertFile
      st = SSL_get_error( (SSL *) this->BIO, st );
      ERR_print_errors_fp( stderr );
      abort();
   }

   if ( SSL_CTX_use_PrivateKey_file( (SSL_CTX *) this->Context, keyFileName, SSL_FILETYPE_PEM ) <= 0 ) {	// set the private key from KeyFile (may be the same as CertFile)
      st = SSL_get_error( (SSL *) this->BIO, st );
      ERR_print_errors_fp( stderr );
      abort();
   }

   if ( ! SSL_CTX_check_private_key( (SSL_CTX *) this->Context ) ) {	// verify private key
      st = SSL_get_error( (SSL *) this->BIO, st );
      ERR_print_errors_fp( stderr );
      abort();
   }
}
 

/**
 *  Connect
 *     use SSL_connect to establish a secure conection
 *
 *  Create a SSL connection
 *
 *  @param	char * hostName, host name
 *  @param	int port, service number
 *
 **/
int SSLSocket::Connect( const char * hostName, int port ) {
   int st;
   st = this->TryToConnect( hostName, port );		// Establish a non ssl connection first
   
   SSL * ssl = (SSL *) this->BIO;
   
   SSL_set_fd(ssl, this->sockId);

   st = SSL_connect(ssl);
   if ( 1 != st ) {
      SSL_get_error( ssl, st );
      throw std::runtime_error("SSLSocket::Connect(const char *, int)");
   }

   return st;
}


/**
 *  Connect
 *     use SSL_connect to establish a secure conection
 *
 *  Create a SSL connection
 *
 *  @param	char * hostName, host name
 *  @param	char * service, service name
 *
 **/
int SSLSocket::Connect( const char * host, const char * service ) {
   int st;
   st = this->TryToConnect( host, service );		// Establish a non ssl connection first
   
   SSL * ssl = (SSL *) this->BIO;
   
   SSL_set_fd(ssl, this->sockId);

   st = SSL_connect(ssl);
   if ( 1 != st ) {
      SSL_get_error( ssl, st );
      fprintf( stderr, "SSL_connect failed with SSL_get_error code %d\n", st );
      throw std::runtime_error("SSLSocket::Connect(const char *, const char *)");
   }

   return st;
}


/**
  *  Read
  *     use SSL_read to read data from an encrypted channel
  *
  *  @param	void * buffer to store data read
  *  @param	size_t size, buffer's capacity
  *
  *  @return	size_t byte quantity read
  *
  *  Reads data from secure channel
  *
 **/
size_t SSLSocket::Read( void * buffer, size_t size ) {
   int st = -1;

   fd_set readfds;
   struct timeval tv;

   FD_ZERO(&readfds);
   FD_SET(this->sockId, &readfds);

   tv.tv_sec = 10;
   tv.tv_usec = 0;

   if (SSL_pending((SSL*)this->BIO) <= 0) {
      st = select(this->sockId + 1, &readfds, NULL, NULL, &tv);

      if (st == -1) {
         throw std::runtime_error("SSLSocket::Read, select");
      } else if (st == 0) {
         throw std::runtime_error("SSLSocket::Read, timeout");
      }
   }

   st = SSL_read( (SSL *) this->BIO, buffer, size);

   if ( -1 == st ) {
      perror("read");
      throw std::runtime_error( "SSLSocket::Read( void *, size_t )" );
   }

   return st;

}


/**
  *  Write
  *     use SSL_write to write data to an encrypted channel
  *
  *  @param	void * buffer to store data read
  *  @param	size_t size, buffer's capacity
  *
  *  @return	size_t byte quantity written
  *
  *  Writes data to a secure channel
  *
 **/
size_t SSLSocket::Write( const char * string ) {
   int st = -1;

   st = SSL_write((SSL *) this->BIO, string, strlen(string));

   if ( -1 == st ) {
      throw std::runtime_error( "SSLSocket::Write( const char * )" );
   }

   return st;

}


/**
  *  Write
  *     use SSL_write to write data to an encrypted channel
  *
  *  @param	const void * buffer to store data to write
  *  @param	size_t size, buffer's capacity
  *
  *  @return	size_t byte quantity written
  *
  *  Reads data from secure channel
  *
 **/
size_t SSLSocket::Write( const void * buffer, size_t size ) {
   int st = -1;

   st = SSL_write((SSL *) this->BIO, buffer, size);

   if ( -1 == st ) {
      throw std::runtime_error( "SSLSocket::Write( void *, size_t )" );
   }

   return st;

}


/**
 *   Show SSL certificates
 *
 **/
void SSLSocket::ShowCerts() {
   X509 *cert;
   char *line;

   cert = SSL_get_peer_certificate( (SSL *) this->BIO );		 // Get certificates (if available)
   if ( nullptr != cert ) {
      printf("Server certificates:\n");
      line = X509_NAME_oneline( X509_get_subject_name( cert ), 0, 0 );
      printf( "Subject: %s\n", line );
      free( line );
      line = X509_NAME_oneline( X509_get_issuer_name( cert ), 0, 0 );
      printf( "Issuer: %s\n", line );
      free( line );
      X509_free( cert );
   } else {
      printf( "No certificates.\n" );
   }

}


/**
 *   Return the name of the currently used cipher
 *
 **/
const char * SSLSocket::GetCipher() {

   return SSL_get_cipher( reinterpret_cast<SSL *>( this->BIO ) );

}

/**
 *   Create constructs a new SSL * variable from a previous created context
 *
 *  @param	Socket * original socket with a previous created context
 *
 **/
void SSLSocket::Copy( SSLSocket * original ) {
   if (this->Context != nullptr) {
      SSL_CTX_free( reinterpret_cast<SSL_CTX *>( this->Context ));
   }
   this->Context = original->Context;
   SSL * ssl = SSL_new( (SSL_CTX *) this->Context );
   if (ssl == nullptr) {
      perror("SSL_new");
      throw std::runtime_error( "SSLSocket::Copy()" );
   }
   this->BIO = (void*) ssl;
   
   int st = SSL_set_fd(ssl, this->sockId);
   
   if (st < 0) {
      perror("SSL_new");
      throw std::runtime_error( "SSLSocket::Copy()" );
   }
}

/**
 *   SSLSocket::Accept
 *
 *  waits for a TLS/SSL client to initiate the TLS/SSL handshake
 *
 **/
void SSLSocket::AcceptSSL(){
   int st = SSL_accept((SSL*) this->BIO);

   if (st <= 0) {
      throw std::runtime_error("SSLSocket::AcceptSSL");
   }

}

VSocket *SSLSocket::AcceptConnection() {
   int id;
   SSLSocket * peer;

   id = this->WaitForConnection();

   peer = new SSLSocket(id);
   if (peer == nullptr) {
      perror("new Socket(id)");
      throw std::runtime_error("SSLSocket::AcceptConnection");
   }
   
   try {
      peer->Copy(this);
      peer->AcceptSSL();
   } catch ( const std::runtime_error& e ) {
      delete peer;
      throw;
   }

   return (VSocket *) peer;
}