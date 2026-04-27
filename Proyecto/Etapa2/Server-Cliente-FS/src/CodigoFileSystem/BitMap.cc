// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#include "BitMap.h"
#include <fstream>
#include <iostream>

// Convierte bloque 0 a bitmap
void convertirBloque0ABitmap(const char* nombreArchivo) {
    // Abrir el archivo en modo binario con permisos de lectura y escritura
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);

    // Posible error
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo" << std::endl;
        return;
    }
    
    // Bitmap necesita 128 bytes para 1024 bloques (1024 bits)
    // Bits en 0 del bitmap
    char bitmap[128] = {0};
    
    // Posicionar el puntero de escritura al inicio del archivo (bloque 0)
    archivo.seekp(0);

    // Escribir el bitmap en el bloque 0
    archivo.write(bitmap, 128);
    
    // Cerrar archivo
    archivo.close();

    // Consola
    std::cout << "Bloque 0 convertido a Bitmap." << std::endl;
}
