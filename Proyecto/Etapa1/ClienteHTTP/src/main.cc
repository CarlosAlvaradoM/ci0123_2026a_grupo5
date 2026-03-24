/**
 * @file main.cpp
 * @brief Programa principal para consultar piezas LEGO desde un servidor web
 * 
 * Este programa permite al usuario seleccionar una figura LEGO y una parte
 * de la misma y muestra las piezas requeridas para construir la figura
 * 
 * @author Carlos Alvarado &&
 * @date 2026
 */

#include <stdio.h>
#include <string.h>

#include "Parser.h"
#include "Socket.h"
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
 * @param argc no utilizado
 * @param argv no utilizado
 * @return int Retorna 0 ejecucion exitosa
 */
int main( int argc, char * argv[] ) {
   const char * os = "http://os.ecci.ucr.ac.cr/";
   const char * osi = "10.84.166.62";
   const char * ose = "163.178.104.62";

   VSocket * s;	
   char buffer[512];
   char request[512];

   // Mostrar el menu
   mostrarMenu();
   // Elejir figura
   int figuraUsuario = elegirFigura();
   // Elejir parte de la figura
   int parteFiguraUsuario = elegirParte();

   const char* figura = obtenerFigura(figuraUsuario);

   // Hacer el request con datos del usuario
   construirRequest(request, obtenerFigura(figuraUsuario), parteFiguraUsuario);

   // Socket
   s = new Socket( 's' );
   s->Connect( ose, 80 );	// usar "osi" en la ECCI, "ose" de sus casas
   s->Write(request);

   // Leer respuesta
   std::string respuesta;
   int bytesRead;

   // Leer socket en bloques
   while ( (bytesRead = s->Read( buffer, 511 )) > 0 ) {
      buffer[bytesRead] = '\0';
      respuesta += buffer;
   }

   // Libera memoria de socket
   delete s;

   // Extraer html
   std::string html = extraerHTML(respuesta);

   // Mostrar los bricks
   imprimirBricks(html, parteFiguraUsuario, figura);
   return 0;

}

