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

int main( int argc, char * argv[] ) {
   const char * os = "http://os.ecci.ucr.ac.cr/";
   const char * osi = "10.84.166.62";
   const char * ose = "163.178.104.62";
   const char * elephant = "GET /lego/list.php?figure=elephant&part=1 HTTP/1.1\r\n"
                        "Host: os.ecci.ucr.ac.cr\r\n"
                        "\r\n";

   VSocket * s;	
   char a[512];

   s = new Socket( 's' );
   s->Connect( ose, 80 );	// usar "osi" en la ECCI, "ose" de sus casas
   s->Write( elephant );
   int bytesRead;
   while ( (bytesRead = s->Read( a, 511 )) > 0 ) {
      a[bytesRead] = '\0'; // Aseguramos que la cadena termine correctamente
      printf( "%s", a);
   }
   s->Read( a, 512 );
   printf( "%s\n", a);

}