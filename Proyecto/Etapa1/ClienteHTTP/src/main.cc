/**
 * @file main.cpp
 * @brief Programa principal para consultar piezas LEGO desde un servidor web
 * 
 * Este programa permite al usuario seleccionar una figura LEGO y una parte
 * de la misma y muestra las piezas requeridas para construir la figura
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
 * @param argv argumentos: --ssl para usar HTTPS, --http para HTTP
 * @return int Retorna 0 ejecucion exitosa
 */
int main( int argc, char * argv[] ) {
   const char * os = "http://os.ecci.ucr.ac.cr/";
   const char * osi = "10.84.166.62";
   const char * ose = "163.178.104.62";
   const char * host = "os.ecci.ucr.ac.cr";
   const char * puerto_http = "80";
   const char * puerto_https = "443";

   VSocket * s;	
   char buffer[512];
   char request[512];
   int usarSSL = 0;

   for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--ssl") == 0) {
         usarSSL = 1;
      }
   }

   mostrarMenu();
   int figuraUsuario = elegirFigura();
   int parteFiguraUsuario = elegirParte();

   const char* figura = obtenerFigura(figuraUsuario);

   construirRequest(request, obtenerFigura(figuraUsuario), parteFiguraUsuario);

   if (usarSSL) {
      s = new SSLsocket('s');
      s->Connect(host, puerto_https);
   } else {
      s = new Socket('s');
      s->Connect(ose, 80);
   }
   
   s->Write(request);

   std::string respuesta;
   int bytesRead;

   while ( (bytesRead = s->Read( buffer, 511 )) > 0 ) {
      buffer[bytesRead] = '\0';
      respuesta += buffer;
   }

   delete s;

   std::string html = extraerHTML(respuesta);

   imprimirBricks(html, parteFiguraUsuario, figura);
   return 0;

}
