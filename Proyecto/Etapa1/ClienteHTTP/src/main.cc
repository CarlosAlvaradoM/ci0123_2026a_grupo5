/**
 * @file main.cpp
 * @brief Programa principal para consultar piezas LEGO desde un servidor web
 * 
 * Este programa permite al usuario seleccionar una figura LEGO y una parte
 * de la misma y muestra las piezas requeridas para construir la figura
 * Soporta IPv4 e IPv6 mediante seleccion en tiempo de ejecucion
 * 
 * @author Carlos Alvarado && Kiara Brenes
 * @date 2026
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include "Parser.h"
#include "Socket.h"
#include "SSLsocket.h"
#include "Menu.h"

/**
 * @brief Funcion para probar la conexion con IPv6
 * 
 * Intenta obtener la lista de figuras para verificar si la conexion IPv6 funciona
 * 
 * @param usarSSL Indica si usar SSL
 * @param hostIPv6 Direccion IPv6
 * @param interfaz Nombre de la interfaz
 * @return true si la conexion es exitosa, false en caso contrario
 */
bool probarConexionIPv6(bool usarSSL, const char* hostIPv6, const char* interfaz) {
    try {
        VSocket* s = nullptr;
        
        if (usarSSL) {
            s = new SSLsocket('s', true);
        } else {
            s = new Socket('s', true);
        }
        
        char direccionCompleta[256];
        if (interfaz != nullptr) {
            snprintf(direccionCompleta, sizeof(direccionCompleta), "%s%%%s", hostIPv6, interfaz);
            if (usarSSL) {
                s->Connect(direccionCompleta, "443");
            } else {
                s->Connect(direccionCompleta, 80);
            }
        } else {
            if (usarSSL) {
                s->Connect(hostIPv6, "443");
            } else {
                s->Connect(hostIPv6, 80);
            }
        }
        
        delete s;
        return true;
    } catch (const std::exception& e) {
        return false;
    } catch (...) {
        return false;
    }
}
 
/**
 * @brief Funcion principal del programa
 * 
 * Flujo de ejecucion:
 * 1. Muestra el menu de figuras
 * 2. Solicita al usuario seleccionar una figura y parte
 * 3. Construye la solicitud HTTP correspondiente
 * 4. Establece conexion con el servidor y envia la solicitud
 * 5. Recibe y almacena la respuesta completa
 * 6. Extrae el cuerpo HTML de la respuesta HTTP
 * 7. Procesa y muestra las piezas LEGO encontradas
 * 
 * @param argc numero de argumentos
 * @param argv argumentos: --ssl para usar HTTPS, --http para HTTP, --ipv6 para usar IPv6
 * @return int Retorna 0 ejecucion exitosa
 */
int main( int argc, char * argv[] ) {
   const char * osi = "10.84.166.62";
   const char * ose = "163.178.104.62";
   const char * host = "os.ecci.ucr.ac.cr";
   const char * puerto_http = "80";
   const char * puerto_https = "443";
   const char * ipv6_addr = "fe80::8f5a:e2e1:7256:ffe3";
   const char * ipv6_interfaz = "enp0s31f6";
 
   VSocket * s;	
   char buffer[512];
   char request[512];
   int usarSSL = 0;
   int usarIPv6 = 0;
   int opcionRed = 0;
 
   // Procesar argumentos de línea de comandos
   for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--ssl") == 0) {
         usarSSL = 1;
         printf("Usando SSL/HTTPS\n");
      }
   }
   
   printf("\n=== Cliente figuras ===\n");
   
   // Ciclo para seleccionar protocolo de red con verificacion
   bool protocoloValido = false;
   while (!protocoloValido) {
       printf("\nSeleccione protocolo de red:\n");
       printf("1. IPv4\n");
       printf("2. IPv6\n");
       
       while (1) {
           printf("Opcion: ");
           if (scanf("%d", &opcionRed) != 1) {
               while (getchar() != '\n');
               printf("Entrada invalida. Por favor ingrese 1 o 2.\n");
               continue;
           }
           if (opcionRed == 1 || opcionRed == 2) {
               break;
           }
           printf("Opcion invalida. Debes elegir un numero entre 1 y 2.\n");
       }
       
       usarIPv6 = (opcionRed == 2);
       
       if (usarIPv6) {
           printf("\nVerificando conectividad IPv6...\n");
           printf("Usando IPv6: %s%%%s\n", ipv6_addr, ipv6_interfaz);
           
           // Probar la conexion antes de continuar
           if (probarConexionIPv6(usarSSL, ipv6_addr, ipv6_interfaz)) {
               printf("La conexion IPv6 fue exitosa\n");
               protocoloValido = true;
           } else {
               printf("Error: No se puede conectar al servidor por IPv6.\n");
               printf("Por favor verifique que este en la red de la ECCI o usando VPN. En caso de no estarlo usar IPv4.\n");
               
               // Limpiar buffer de entrada
               while (getchar() != '\n');
               printf("Presione Enter para continuar");
               getchar();
               continue;
           }
       } else {
           printf("\nUsando IPv4\n");
           protocoloValido = true;
       }
   }
   
   // Obtener lista de figuras del servidor usando el protocolo seleccionado
   printf("\nObteniendo la lista de figuras del servidor...\n");
   
   int numFiguras;
   if (usarIPv6) {
      numFiguras = obtenerListaFigurasDelServidor(true, usarSSL, ipv6_addr, ipv6_interfaz);
   } else {
      numFiguras = obtenerListaFigurasDelServidor(false, usarSSL);
   }
   
   if (numFiguras <= 0) {
      printf("Advertencia: No se pudo obtener lista de figuras del servidor.\n");
      printf("Usando lista por defecto.\n");
   } else {
      printf("\nSe encontraron %d figuras en el servidor.\n", numFiguras);
   }
   
   // Mostrar menu y seleccionar figura
   mostrarMenu();
   int figuraUsuario = elegirFigura();
   int parteFiguraUsuario = elegirParte();
 
   const char* figura = obtenerFigura(figuraUsuario);
 
   construirRequest(request, obtenerFigura(figuraUsuario), parteFiguraUsuario);
   
   // Conectar al servidor para la consulta
   try {
      if (usarSSL) {
         s = new SSLsocket('s', usarIPv6);
         if (usarIPv6) {
            char direccionCompleta[256];
            snprintf(direccionCompleta, sizeof(direccionCompleta), "%s%%%s", ipv6_addr, ipv6_interfaz);
            s->Connect(direccionCompleta, puerto_https);
         } else {
            s->Connect(host, puerto_https);
         }
      } else {
         s = new Socket('s', usarIPv6);
         if (usarIPv6) {
            char direccionCompleta[256];
            snprintf(direccionCompleta, sizeof(direccionCompleta), "%s%%%s", ipv6_addr, ipv6_interfaz);
            s->Connect(direccionCompleta, 80);
         } else {
            s->Connect(ose, 80);
         }
      }
   } catch (const std::exception& e) {
      printf("\nError al conectar al servidor:\n");
      printf("Detalle: %s\n\n", e.what());
      liberarListaFiguras();
      return 1;
   } catch (...) {
      printf("\nError desconocido al conectar al servidor.\n\n");
      liberarListaFiguras();
      return 1;
   }
   
   printf("\nEnviando solicitud...\n");
   s->Write(request);
 
   std::string respuesta;
   int bytesRead;
 
   printf("Recibiendo respuesta del servidor...\n");
   while ( (bytesRead = s->Read( buffer, 511 )) > 0 ) {
      buffer[bytesRead] = '\0';
      respuesta += buffer;
   }
 
   delete s;
 
   std::string html = extraerHTML(respuesta);
 
   imprimirBricks(html, parteFiguraUsuario, figura);
   
   liberarListaFiguras();
   return 0;
}