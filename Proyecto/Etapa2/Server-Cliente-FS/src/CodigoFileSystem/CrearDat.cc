// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#include "CrearDat.h"
#include <fstream>
#include <iostream>

// Crear archivo
void crearDat(const char* nombreArchivo) {
    // 256KB = 262144 bytes
    // Bloques de 256 bytes = 1024 bloques
    const int tamanoTotal = 256 * 1024;
    
    // Abrir el archivo en modo binario (esto sobrescribe si ya existe)
    std::ofstream archivo(nombreArchivo, std::ios::binary);
    
    // Posible error
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear el archivo" << std::endl;
        return;
    }
    
    // Crear archivo lleno de ceros
    char cero = 0;
    for (int i = 0; i < tamanoTotal; i++) {
        archivo.write(&cero, 1);
    }
    
    // Cerrar archivo
    archivo.close();

    // Consola
    std::cout << "Archivo " << nombreArchivo << " creado. Tamaño: 256KB" << std::endl;
}
