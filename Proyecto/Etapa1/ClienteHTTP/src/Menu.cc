#include <stdio.h>
#include "Menu.h"

// Lista de figuras en el server
static const char* figuras[] = {
   "horse",
   "elephant",
   "giraffe",
   "whitesheep",
   "blacksheep",
   "duck",
   "fish"
};

static const int TOTAL_FIGURAS = 7;

void mostrarMenu() {
   // Consola
   printf("Menu de figuras en el servidor OS\n");
   // Recorrer figuras para mostrar las opciones de animales
   for (int i = 0; i < TOTAL_FIGURAS; i++) {
      printf("%d. %s\n", i + 1, figuras[i]);
   }
}

int elegirFigura() {
   // Figura a elejir
   int opcion;
   // Enciclar al usuario hasta que elija opcion valida
   do {
      // Consola
      printf("Seleccione una figura: ");
      // Guardar opcion
      scanf("%d", &opcion);
     // Comprobar respuesta 
   } while (opcion < 1 || opcion > TOTAL_FIGURAS);

   // Retornar
   return opcion - 1;
}

int elegirParte() {
   // Parte de la figura
   int parte;
   // Enciclar si no digita opcion correcta
   do {
      // Consola
      printf("Seleccione la parte (1 o 2): ");
      // Guardar
      scanf("%d", &parte);
     // Comprobar 
   } while (parte != 1 && parte != 2);

   // Retornar respuesta
   return parte;
}

const char* obtenerFigura(int index) {
   // Relacionar figura con posicion en lista
   return figuras[index];
}

void construirRequest(char* buffer, const char* figura, int parte) {
   // Construir el GET
   sprintf(buffer,
      "GET /lego/list.php?figure=%s&part=%d HTTP/1.1\r\n"
      "Host: os.ecci.ucr.ac.cr\r\n"
      "\r\n",
      figura, parte);
}