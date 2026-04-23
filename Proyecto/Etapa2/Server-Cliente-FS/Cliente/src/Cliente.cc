// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
#include <stdexcept>

#include "Cliente.h"
#include "Socket.h"

/**
 * @brief Constructor por argumentos
 */
Cliente::Cliente(const std::string& dir)
  : dir(dir) {
}

/**
 * @brief Rutina para pedir las piezas para armar una figura e imprimir
 * la lista en la salida.
 * @returns Un código de error. 0 en caso de éxito, -1 al contrario;
 */
int Cliente::pedirFigura() {
//   char figura[16];

//   printf("Ingrese el nombre de la figura que quiere buscar\n");
//   scanf("%15s", figura);

//   for (char& c : figura) {
//     c = (char) (std::tolower((int) c));
//   }

//   // Verificamos si la solicitud es para pedir la cantidad total de figuras
//   bool listaFiguras = strcmp(figura, "totalfigures") == 0;
//   Parser p;
//   std::string response;

//   try {
//     if (listaFiguras) {
//       // Si es la lista de figuras, mandamos una única solicitud
//       response += this->pedirPagina(figura);
//     } else {
//       // Si es la lista de piezas, mandamos 2 solicitudes para las 2 páginas
//       response += this->pedirPagina(figura, 1);
//       response += this->pedirPagina(figura, 2);
//     }
//   }
//   catch(const std::exception& e) {
//     fprintf(stderr, "%s", e.what());
//     return -1;
//   }
//   printf("%s\n", response.c_str());
  
//   if (!p.parseHttpResponse(response)) {
//     printf("Error, request invalida. Figura no existente\n");
//     exit(-1);
//   }
  
//   // Se obtiene la lista de respuestas, para posteriormente imprimirlas.
//   const std::vector<std::string>& piezas = p.getLista();

//   if (listaFiguras) {
//     printf("La lista de piezas disponibles en este servidor son: \n");
//   } else {
//     printf("Piezas necesarias para %s\n", figura);
//   }
  
//   for (const std::string& s : piezas) {
//     printf("%s\n", s.c_str());
//   }
  
//   if (!listaFiguras) {
//     printf("Y así se arma un %s!\n", figura);
//   }  
//   return 0;
// }

// /**
//  * @brief Metodo privado para armar el mensaje del request
//  * @param figura lo que se quiere solicitar
//  * @param pagina nro de pagina
//  */
// std::string Cliente::formarRequest(const std::string& figura, int pagina) {
//   std::string request;
//   request += "GET /lego/";
//   if (figura == "totalfigures") {
//     request += "index.php";
//   } else {
//     if (pagina <= 0 || pagina > 2) {
//       throw std::invalid_argument("Numero de pagina no valido\n");
//     }

//     request += "list.php?figure="
//               + figura
//               + "&part="
//               + std::to_string(pagina);
//   }
//   request += " HTTP/1.1\r\nHost: "
//              "os.ecci.ucr.ac.cr"  // Por el momento este será el host fijo
//              "\r\nConnection: close\r\n\r\n";  // El servidor de por si
//              // Cierra la conexión una vez responde, por lo que se le indica
//              // que por cada solicitud, cierre la conexión
//   return request;
return 0;
}

std::string Cliente::pedirPagina(const std::string& figura, int pagina) {
  // // Se crea un socket SSL para cada solicitud
  // SSLSocket sock;
  // // Como se está usando sockets SSL, usamos el servicio https
  // // Por el momento solo IPv4
  // sock.Connect(this->dir.c_str(), "https");

  // std::string req = this->formarRequest(figura, pagina);
  // sock.Write(req.c_str(), req.size());

  // std::string response;
  // char buffer[512];
  // int n;

  // // Leemos el buffer completo de la solicitud https
  // // TODO Cambiar a memoria dinámica en caso de que el cuerpo sea muy grande
  // while ((n = sock.Read(buffer, sizeof(buffer))) > 0) {
  //   response.append(buffer, n);
  // }

  // return response;
  return "";
}
