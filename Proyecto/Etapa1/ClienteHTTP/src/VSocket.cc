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

void VSocket::Init( char t, bool IPv6 ){

   int st;
   int domain;
   int type;

   // Guardar si es IPv6
   this->IPv6 = IPv6;

   // Ver dominio
   if (IPv6) {
      domain = AF_INET6;
   } else {
      domain = AF_INET;
   }

   // Determinar tipo de socket
   if (t == 's') {
      type = SOCK_STREAM;  // TCP
   } else if (t == 'd') {
      type = SOCK_DGRAM;  // UDP
   } else {
      throw std::runtime_error("VSocket::Init, tipo inválido (use 's' o 'd')");
   }

   // Crear el socket con la llamada al sistema
   st = socket(domain, type, 0);

   // Error
   if ( st == -1 ) {
      throw std::runtime_error( "VSocket::Init, (reason)" );
   }

   // Guardar en el descriptor
   this->sockId = st;

}

VSocket::~VSocket() {

   this->Close();

}

void VSocket::Close(){

   if (this->sockId != -1) {
      int st = close(this->sockId);

      if (st == -1) {
         throw std::runtime_error("VSocket::Close, error al cerrar socket");
      }

      this->sockId = -1;  // Marcar como cerrado
   }

}

int VSocket::TryToConnect(const char* hostip, int port) {
    int st;

    // IPv6, usar GETADDRINFO
    if (this->IPv6) {
        struct addrinfo hints;
        struct addrinfo* result;
        struct addrinfo* rp;
        
        // Convertir puerto a string
        char port_str[16];
        snprintf(port_str, sizeof(port_str), "%d", port);
        
        // Configurar criterios para la resolucion de direcciones IPv6
        memset(&hints, 0, sizeof(hints));
        // Forzar IPV6
        hints.ai_family = AF_INET6;
        // Socket TCP, stream
        hints.ai_socktype = SOCK_STREAM;
        // hostip ya es una dirección numérica
        hints.ai_flags = AI_NUMERICHOST;
        
        // Resolver direccion
        st = getaddrinfo(hostip, port_str, &hints, &result);
    
        // Error
        if (st != 0) {
            throw std::runtime_error("VSocket::TryToConnect IPv6: getaddrinfo falló");
        }
        
        // Intentar conectar con cada dirección obtenida
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            st = connect(this->sockId, rp->ai_addr, rp->ai_addrlen);
            // Exito
            if (st == 0) {
                break;
            }
        }

        // Liberar memoria asignada por getaddrinfo
        freeaddrinfo(result);
        
        if (st == -1) {
            throw std::runtime_error("VSocket::TryToConnect IPv6: connect falló");
        }

    // IPV4
    } else {
        struct sockaddr_in host4;
        memset(&host4, 0, sizeof(host4));
        host4.sin_family = AF_INET;

        // Convertir direccion IPv4 de texto a formato binario
        st = inet_pton(AF_INET, hostip, &host4.sin_addr);
        if (st <= 0) {
            throw std::runtime_error("VSocket::TryToConnect: inet_pton falló para IPv4");
        }

        // Convertir puerto a network byte order
        host4.sin_port = htons(port);

        // Intentar establecer la conexion
        st = connect(this->sockId, (struct sockaddr*)&host4, sizeof(host4));
        if (st == -1) {
            throw std::runtime_error("VSocket::TryToConnect: connect falló para IPv4");
        }
    }

    // Guardar el puerto usado en el objeto
    this->port = port;
    return st;
}

int VSocket::TryToConnect( const char *host, const char *service ) {
    int st;
    struct addrinfo hints;
    struct addrinfo* result;
    struct addrinfo* rp;
    
    // Configurar criterios de busqueda
    memset(&hints, 0, sizeof(hints));
    
    // Establecer familia según ipv6 o ipv4
    if (this->IPv6) {
        hints.ai_family = AF_INET6;
    } else {
        hints.ai_family = AF_INET;
    }
    
    // Especificar tipo de socket, TCP
    hints.ai_socktype = SOCK_STREAM;
    
    // Resolver nombre (DNS lookup), convierte hostname a IP
    st = getaddrinfo(host, service, &hints, &result);
    if (st != 0) {
        throw std::runtime_error("VSocket::TryToConnect DNS: getaddrinfo falló");
    }
    
    // Intentar conectar con cada direccion obtenida
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        st = connect(this->sockId, rp->ai_addr, rp->ai_addrlen);
        // Exito
        if (st == 0) {
            break;
        }
    }
    
    // Liberar memoria
    freeaddrinfo(result);
    
    // Error
    if (st == -1) {
        throw std::runtime_error("VSocket::TryToConnect DNS: no se pudo conectar a ninguna dirección");
    }
    
    return st;
}

