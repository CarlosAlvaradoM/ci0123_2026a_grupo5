#include <stdio.h>
#include <string.h>

#include "Parser.h"
#include "Socket.h"
#include "Menu.h"

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

   while ( (bytesRead = s->Read( buffer, 511 )) > 0 ) {
      buffer[bytesRead] = '\0';
      respuesta += buffer;
   }

   delete s;

   // Extraer html
   std::string html = extraerHTML(respuesta);

   // Mostrar los bricks
   imprimirBricks(html, parteFiguraUsuario, figura);
   return 0;

}

