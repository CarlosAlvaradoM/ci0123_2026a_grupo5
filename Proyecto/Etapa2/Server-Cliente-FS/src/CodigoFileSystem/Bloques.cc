// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#include "Bloques.h"
#include <fstream>
#include <iostream>

// Limpiar bloques
void limpiarBloquesDesde2(const char* nombreArchivo) {
    // Abrir el archivo en modo binario con permisos de lectura y escritura
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
    
    // Posible error
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo" << std::endl;
        return;
    }
    
    // Parametros
    const int totalBloques = 1024;
    const int tamanoBloque = 256;
    
    // Desde bloque 2 hasta el final (bloque 1023)
    for (int bloque = 2; bloque < totalBloques; bloque++) {
        char bloqueVacio[tamanoBloque] = {0};
        // Posicionar el puntero al inicio del bloque actual
        archivo.seekp(bloque * tamanoBloque);
        // Escribir el bloque lleno de ceros
        archivo.write(bloqueVacio, tamanoBloque);
    }
    
    // Cerrar archivo
    archivo.close();

    // Consola
    std::cout << "Bloques 2 al " << (totalBloques - 1) << " limpiados (puestos en 0)." << std::endl;
}
