// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#include "ManipularDat.h"
#include <fstream>
#include <iostream>
#include <cstring>

// Estructura de inodo
struct Inodo {
    char nombre[8];  // Nombre de la figura (hasta 7 caracteres)
    int tamano;  // Tamaño total en bytes del contenido
    short primerBloque;  // Número del primer bloque de datos
    char activo;  // 1 = activo (figura válida), 0 = libre
    char reservado;  // Alineación (siempre 0)
};

// Estructura de bloque de datos
struct BloqueDatos {
    char datos[240];  // Contenido de la figura en este bloque
    short siguienteBloque; // Número del siguiente bloque (65535 = EOF)
};

// Leer una figura
std::string leerFigura(const char* nombreArchivo, const char* nombreFigura) {

    // Abrir archivo en modo binario solo lectura
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::binary);
    
    // Posible error
    if (!archivo.is_open()) {
        return "Error: No se pudo abrir el archivo\n";
    }
    
    // Crear inodo
    Inodo inodo;
    int inodoEncontrado = -1;
    
    // Recorrer los 16 inodos en el bloque 1 (bytes 256-511)
    for (int i = 0; i < 16; i++) {
        archivo.seekg(256 + i * 16);
        archivo.read(reinterpret_cast<char*>(&inodo), 16);
        
        // Verificar si es el inodo activo que se busca
        if (inodo.activo == 1 && strcmp(inodo.nombre, nombreFigura) == 0) {
            inodoEncontrado = i;
            break;
        }
    }
    
    // Error
    if (inodoEncontrado == -1) {
        archivo.close();
        return "Figura no encontrada\n";
    }
    
    // Cabecera
    std::string resultado = "";
    resultado += "Leyendo figura: ";
    resultado += nombreFigura;
    resultado += "\n----------------------------------------\n";
    
    // Recorrer la lista en lazada de bloques
    unsigned short bloqueActual = inodo.primerBloque;
    int bytesLeidos = 0;
    int contadorBloques = 0;
    
    // Hasta eof
    while (bloqueActual != 65535 && bytesLeidos < inodo.tamano) {
        contadorBloques++;
        BloqueDatos bloque;
        
        // Leer bloque completo
        archivo.seekg(bloqueActual * 256);
        archivo.read(reinterpret_cast<char*>(&bloque), 242);
        
        // Extraer hasta 240 bytes
        for (int i = 0; i < 240 && bytesLeidos < inodo.tamano; i++) {
            if (bloque.datos[i] != '\0') {
                resultado += bloque.datos[i];
                bytesLeidos++;
            }
        }
        
        // Avanzar al siguiente bloque
        bloqueActual = bloque.siguienteBloque;
    }
    
    resultado += "\n----------------------------------------\n";
    resultado += "Bloques usados: " + std::to_string(contadorBloques) + "\n";
    resultado += "Bytes leidos: " + std::to_string(bytesLeidos) + "\n";
    
    archivo.close();
    return resultado;
}

// Escribir en bloques
void escribirStringEnBloquesLibres(const char* nombreArchivo, const char* nombreFigura, const char* string) {
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
    
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo" << std::endl;
        return;
    }
    
    // Verificar longitud del nombre
    int longitudNombre = strlen(nombreFigura);
    if (longitudNombre >= 8) {
        std::cout << "Error: El nombre '" << nombreFigura << "' es muy largo. Maximo 7 caracteres." << std::endl;
        archivo.close();
        return;
    }
    
    int longitudTotal = strlen(string);
    std::cout << "Escribiendo figura '" << nombreFigura << "' de " << longitudTotal << " bytes..." << std::endl;
    
    // Verificar si ya existe
    for (int i = 0; i < 16; i++) {
        Inodo temp;
        archivo.seekg(256 + i * 16);
        archivo.read(reinterpret_cast<char*>(&temp), 16);
        
        if (temp.activo == 1 && strcmp(temp.nombre, nombreFigura) == 0) {
            std::cout << "Error: Ya existe una figura con el nombre '" << nombreFigura << "'" << std::endl;
            archivo.close();
            return;
        }
    }
    
    // Leer bitmap
    char bitmap[128];
    archivo.seekg(0);
    archivo.read(bitmap, 128);
    
    // Calcular bloques a necesitar, redondea arriba
    int bloquesNecesarios = (longitudTotal + 239) / 240;
    std::cout << "Bloques necesarios: " << bloquesNecesarios << std::endl;
    
    // Buscar bloques libres
    // Asumir max 100 bloques
    int bloquesLibres[100];
    int encontrados = 0;
    
    for (int bloque = 2; bloque < 1024 && encontrados < bloquesNecesarios; bloque++) {
        int byteIndex = bloque / 8;
        int bitIndex = bloque % 8;
        
        if (!((bitmap[byteIndex] >> bitIndex) & 1)) {
            bloquesLibres[encontrados] = bloque;
            encontrados++;
        }
    }
    
    if (encontrados < bloquesNecesarios) {
        std::cout << "Error: No hay suficientes bloques libres. Necesito " << bloquesNecesarios 
                  << " pero solo hay " << encontrados << std::endl;
        archivo.close();
        return;
    }
    
    std::cout << "Bloques asignados: ";
    for (int i = 0; i < bloquesNecesarios; i++) {
        std::cout << bloquesLibres[i] << " ";
    }
    std::cout << std::endl;
    
    // Escribir los datos en los bloques enlazados
    int bytesEscritos = 0;
    
    for (int i = 0; i < bloquesNecesarios; i++) {
        BloqueDatos bloque;
        memset(&bloque, 0, 242);
        
        // Bytes escribir en este bloque
        int bytesEnEsteBloque = longitudTotal - bytesEscritos;
        if (bytesEnEsteBloque > 240) bytesEnEsteBloque = 240;
        
        // Copiar los datos
        memcpy(bloque.datos, string + bytesEscritos, bytesEnEsteBloque);
        
        // Puntero al siguiente bloque o EOF si es el ultimo
        if (i == bloquesNecesarios - 1) {
            // EOF
            bloque.siguienteBloque = 65535;
        } else {
            bloque.siguienteBloque = bloquesLibres[i + 1];
        }
        
        // Escribir el bloque
        archivo.seekp(bloquesLibres[i] * 256);
        archivo.write(reinterpret_cast<char*>(&bloque), 242);
        
        // Marcar bloque como ocupado en bitmap
        int byteIndex = bloquesLibres[i] / 8;
        int bitIndex = bloquesLibres[i] % 8;
        bitmap[byteIndex] |= (1 << bitIndex);
        
        bytesEscritos += bytesEnEsteBloque;
    }
    
    // Guardar bitmap actualizado
    archivo.seekp(0);
    archivo.write(bitmap, 128);
    
    // Crear el inodo
    Inodo nuevoInodo;
    memset(&nuevoInodo, 0, 16);
    strcpy(nuevoInodo.nombre, nombreFigura);
    nuevoInodo.tamano = longitudTotal;
    // Primer bloque
    nuevoInodo.primerBloque = bloquesLibres[0];
    nuevoInodo.activo = 1;
    
    // Buscar inodo libre
    int inodoLibre = -1;
    for (int i = 0; i < 16; i++) {
        Inodo temp;
        archivo.seekg(256 + i * 16);
        archivo.read(reinterpret_cast<char*>(&temp), 16);
        if (temp.activo == 0) {
            inodoLibre = i;
            break;
        }
    }
    
    // Si hay inodo libre, escribirlo; si no, mostrar error
    if (inodoLibre != -1) {
        archivo.seekp(256 + inodoLibre * 16);
        archivo.write(reinterpret_cast<char*>(&nuevoInodo), 16);
        std::cout << "Figura '" << nombreFigura << "' escrita correctamente en " 
                  << bloquesNecesarios << " bloques" << std::endl;
    } else {
        std::cout << "Error: No hay inodos libres" << std::endl;
    }
    
    archivo.close();
}


std::string ListData(const char* nombreArchivo) {
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::binary);
    std::string listaNombres = "";
    
    if (!archivo.is_open()) {
        return "Error: No se pudo abrir el archivo";
    }

    Inodo inodo;

    // Recorrer los 16 inodos en el Bloque 1 (posición 256)
    for (int i = 0; i < 16; i++) {
        archivo.seekg(256 + i * sizeof(Inodo));
        archivo.read(reinterpret_cast<char*>(&inodo), sizeof(Inodo));
        
        // Si el inodo está marcado como activo, agregar su nombre a la lista
        if (inodo.activo == 1) {
            listaNombres += std::string(inodo.nombre) + "\n";
        }
    }

    archivo.close();
    
    // Si no se encontraron figuras, retornar mensaje
    if (listaNombres.empty()) {
        return "No hay figuras guardadas.";
    }

    return listaNombres;
}