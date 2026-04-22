#include "Inodos.h"
#include <fstream>
#include <iostream>
#include <cstring>

struct Inodo {
    char nombre[8]; // Nombre del archivo
    int tamano; // Tamaño en bytes
    short primerBloque; // Bloque donde empieza
    char activo; // 1 = activo, 0 = libre
    char reservado; // Para alineacion
};

void convertirBloque1AInodo(const char* nombreArchivo) {
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
    
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo" << std::endl;
        return;
    }
    
    // Bloque 1, posicion 256
    // Capacidad para 16 inodos (16 * 16 = 256 bytes)
    
    Inodo inodosVacios[16];
    
    // Inicializar todos los inodos como libres
    for (int i = 0; i < 16; i++) {
        memset(inodosVacios[i].nombre, 0, 8);
        inodosVacios[i].tamano = 0;
        inodosVacios[i].primerBloque = 0;
        inodosVacios[i].activo = 0;
        inodosVacios[i].reservado = 0;
    }
    
    // Escribir en la posicion 256, inicio del bloque 1
    archivo.seekp(256);
    archivo.write(reinterpret_cast<char*>(inodosVacios), 256);
    
    archivo.close();
    std::cout << "Bloque 1 convertido a tabla de inodos. 16 inodos vacios." << std::endl;
}
