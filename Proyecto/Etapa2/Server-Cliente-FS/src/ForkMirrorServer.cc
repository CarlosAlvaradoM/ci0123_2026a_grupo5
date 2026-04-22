/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2026-i
 *  Grupos: 2 y 3
 *
 *   Socket client/server example
 *
 * (Fedora version)
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h> // memset
#include <unistd.h>

#include <string>
#include <algorithm>

#include <fstream>

#include "Servidor-Cliente/Socket.h"
#include "CodigoFileSystem/ManipularDat.h"
#include "CodigoFileSystem/CrearDat.h"
#include "CodigoFileSystem/BitMap.h"
#include "CodigoFileSystem/Inodos.h"
#include "CodigoFileSystem/Bloques.h"
#include "CodigoFileSystem/LlenarFiguras.h"

#define PORT 2026
#define BUFSIZE 512
#define RUTA_FS "CodigoFileSystem/figuras.dat"

bool existeArchivo(const std::string& nombre);

int main(int argc, char **argv) {

   // Verificar si el archivo figuras.dat existe
   bool existeFiguras = existeArchivo(RUTA_FS);

   // Crear o no archivo figuras.dat
   if(!existeFiguras) {
      // Llamar a funcion que formatea el disco
      crearDat(RUTA_FS);
      convertirBloque0ABitmap(RUTA_FS);
      convertirBloque1AInodo(RUTA_FS);
      limpiarBloquesDesde2(RUTA_FS);

      // Llamar a funcion de poner las figuras en el disco
      ponerFiguras(RUTA_FS);
   }

   VSocket *s1, *s2;
   int childpid;
   char a[BUFSIZE];

   s1 = new Socket('s'); // Create a stream IPv4 socket

   s1->Bind(PORT);     // Port to access this mirror server
   s1->MarkPassive(5); // Set passive socket and backlog queue to 5 connections

   for (;;) {
      s2 = s1->AcceptConnection(); // Wait for a new connection, connection info is in s2 variable
      childpid = fork();           // Create a child to serve the request
      if (childpid < 0) {
         perror("server: fork error");
      }
      else {
         if (0 == childpid) {               // child code
            s1->Close(); // Close original socket "s1" in child
            memset(a, 0, BUFSIZE);
            s2->Read(a, BUFSIZE);

            // Pasar a string
            std::string request(a);
            // Respuesta de FS
            std::string respuesta = "";

            // Convertir a mayusculas el comando
            std::string comando;
            std::string argumento;

            size_t pos = request.find(" ");

            if (pos != std::string::npos) {
               comando = request.substr(0, pos);
               argumento = request.substr(pos + 1);
            } else {
               comando = request;
            }

            // Uppercase comando
            std::transform(comando.begin(), comando.end(), comando.begin(), ::toupper);

            if (comando == "LISTDATA") {
               respuesta = ListData(RUTA_FS);
            }
            else if (comando == "GET") {
               respuesta = leerFigura(RUTA_FS, argumento.c_str());
            }
            else {
               respuesta = "ERROR: comando no reconocido";
            }

            printf("Servidor recibió solicitud: %s\n", request.c_str());

            s2->Write(respuesta.c_str());
            exit(0); // Exit, finish child work
         }
      }

      s2->Close(); // Close socket s2 in parent, then go wait for a new conection
   }
}

bool existeArchivo(const std::string& nombre) {
   std::ifstream archivo(nombre.c_str());
   return archivo.good();
}