/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *
  *
  *  Cliente
  *
 **/

#include <stdio.h>
#include <string.h>

#include "VSocket.h"
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

   // Hacer el request con datos del usuario
   construirRequest(request, obtenerFigura(figuraUsuario), parteFiguraUsuario);

   s = new Socket( 's' );

   s->Connect( ose, 80 );	// usar "osi" en la ECCI, "ose" de sus casas

   s->Write(request);

   int bytesRead;

   while ( (bytesRead = s->Read( buffer, 511 )) > 0 ) {
      buffer[bytesRead] = '\0'; // Aseguramos que la cadena termine correctamente
      printf( "%s", buffer);
   }

   delete s;
   return 0;

}

