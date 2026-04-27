#include "CrearDat.h"
#include <fstream>
#include <iostream>

void crearDat(const char* nombreArchivo) {
    // 256KB = 262144 bytes
    // Bloques de 256 bytes = 1024 bloques
    const int tamanoTotal = 256 * 1024;
    
    std::ofstream archivo(nombreArchivo, std::ios::binary);
    
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear el archivo" << std::endl;
        return;
    }
    
    // Crear archivo lleno de ceros
    char cero = 0;
    for (int i = 0; i < tamanoTotal; i++) {
        archivo.write(&cero, 1);
    }
    
    archivo.close();
    std::cout << "Archivo " << nombreArchivo << " creado. Tamaño: 256KB" << std::endl;
}
