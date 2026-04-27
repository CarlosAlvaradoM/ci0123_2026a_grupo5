/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2026-i
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "Socket.h"
#include "CodigoFileSystem/ManipularDat.h"
#include "CodigoFileSystem/CrearDat.h"
#include "CodigoFileSystem/BitMap.h"
#include "CodigoFileSystem/Inodos.h"
#include "CodigoFileSystem/Bloques.h"
#include "CodigoFileSystem/LlenarFiguras.h"

#define PORT 2026
#define BUFSIZE 512
#define RUTA_FS "../src/CodigoFileSystem/figuras.dat"

bool existeArchivo(const std::string &nombre);

int main(int argc, char **argv) {

   (void)argc;
   (void)argv;

   // Inicializar FS si no existe
   if (!existeArchivo(RUTA_FS)) {
      crearDat(RUTA_FS);
      convertirBloque0ABitmap(RUTA_FS);
      convertirBloque1AInodo(RUTA_FS);
      limpiarBloquesDesde2(RUTA_FS);
      ponerFiguras(RUTA_FS);
   }

   VSocket *s1, *s2;
   int childpid;

   bool IPv6 = true;
   s1 = new Socket('s' , IPv6);
   s1->Bind(PORT);
   s1->MarkPassive(5);

   for (;;) {
      s2 = s1->AcceptConnection();

      childpid = fork();

      if (childpid < 0) {
         perror("server: fork error");
      }
      // Proceso hijo
      else if (childpid == 0) {
         s1->Close();

         char buffer[BUFSIZE];
         int bytes;
         std::string request;

         while (true) {
            request.clear();

            // Leer request completo
            while ((bytes = s2->Read(buffer, BUFSIZE)) > 0) {
               request.append(buffer, bytes);

               if (request.find("\r\n\r\n") != std::string::npos) {
                  break;
               }
            }

            // Cliente cerro conexion
            if (request.empty()) break;

            printf("Request recibido:\n%s\n", request.c_str());

            // Parser 
            std::string metodo;
            std::string ruta;

            size_t endLine = request.find("\r\n");
            std::string firstLine = request.substr(0, endLine);

            std::stringstream ss(firstLine);
            ss >> metodo >> ruta;

            std::transform(metodo.begin(), metodo.end(), metodo.begin(), ::toupper);

            // Logica
            std::string respuesta;

            if (metodo == "GET") {
               if (ruta == "/") {
                  respuesta = ListData(RUTA_FS);
               } else {
                  // Quitar el /
                  std::string nombreFigura = ruta.substr(1);
                  respuesta = leerFigura(RUTA_FS, nombreFigura.c_str());
               }
            } else {
               respuesta = "Metodo no soportado";
            }

            // Respuesta HTTP
            std::string httpResponse;

            if (respuesta.find("no encontrada") != std::string::npos) {
               httpResponse =
                  "HTTP/1.1 404 Not Found\r\n"
                  "Content-Type: text/plain\r\n\r\n" +
                  respuesta;
            } else {
               httpResponse =
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/plain\r\n\r\n" +
                  respuesta;
            }

            // Enviar respuesta (incluye '\0' para cliente)
            s2->Write(httpResponse.c_str(), httpResponse.size() + 1);
         }

         s2->Close();
         exit(0);
      }

      // Proceso padre
      s2->Close();
   }
}

bool existeArchivo(const std::string &nombre) {
   std::ifstream archivo(nombre.c_str());
   return archivo.good();
}