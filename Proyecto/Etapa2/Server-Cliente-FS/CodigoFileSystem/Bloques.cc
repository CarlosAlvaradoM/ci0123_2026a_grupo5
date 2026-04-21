#include "Bloques.h"
#include <fstream>
#include <iostream>

void limpiarBloquesDesde2(const char* nombreArchivo) {
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
    
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo" << std::endl;
        return;
    }
    
    const int totalBloques = 1024;
    const int tamanoBloque = 256;
    
    // Desde bloque 2 hasta el final (bloque 1023)
    for (int bloque = 2; bloque < totalBloques; bloque++) {
        char bloqueVacio[tamanoBloque] = {0};
        archivo.seekp(bloque * tamanoBloque);
        archivo.write(bloqueVacio, tamanoBloque);
    }
    
    archivo.close();
    std::cout << "Bloques 2 al " << (totalBloques - 1) << " limpiados (puestos en 0)." << std::endl;
}
