#include "BitMap.h"
#include <fstream>
#include <iostream>

void convertirBloque0ABitmap(const char* nombreArchivo) {
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
    
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo" << std::endl;
        return;
    }
    
    // Bitmap necesita 128 bytes para 1024 bloques (1024 bits)

    // Bits en 0 del bitmap
    char bitmap[128] = {0};
    
    // Escribir el bitmap en la posición 0
    archivo.seekp(0);
    archivo.write(bitmap, 128);
    
    archivo.close();
    std::cout << "Bloque 0 convertido a Bitmap." << std::endl;
}
