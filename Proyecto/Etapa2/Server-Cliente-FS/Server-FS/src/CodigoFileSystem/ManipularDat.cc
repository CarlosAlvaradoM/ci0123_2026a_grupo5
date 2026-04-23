#include "ManipularDat.h"
#include <fstream>
#include <iostream>
#include <cstring>

struct Inodo {
    char nombre[8];
    int tamano;
    short primerBloque;
    char activo;
    char reservado;
};

struct BloqueDatos {
    char datos[240];
    short siguienteBloque;
};

std::string leerFigura(const char* nombreArchivo, const char* nombreFigura) {

    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::binary);
    
    if (!archivo.is_open()) {
        return "Error: No se pudo abrir el archivo\n";
    }
    
    Inodo inodo;
    int inodoEncontrado = -1;
    
    for (int i = 0; i < 16; i++) {
        archivo.seekg(256 + i * 16);
        archivo.read(reinterpret_cast<char*>(&inodo), 16);
        
        if (inodo.activo == 1 && strcmp(inodo.nombre, nombreFigura) == 0) {
            inodoEncontrado = i;
            break;
        }
    }
    
    if (inodoEncontrado == -1) {
        archivo.close();
        return "Figura no encontrada\n";
    }
    
    std::string resultado = "";
    resultado += "Leyendo figura: ";
    resultado += nombreFigura;
    resultado += "\n----------------------------------------\n";
    
    short bloqueActual = inodo.primerBloque;
    int bytesLeidos = 0;
    int contadorBloques = 0;
    
    while (bloqueActual != 65535 && bytesLeidos < inodo.tamano) {
        contadorBloques++;
        BloqueDatos bloque;
        
        archivo.seekg(bloqueActual * 256);
        archivo.read(reinterpret_cast<char*>(&bloque), 242);
        
        for (int i = 0; i < 240 && bytesLeidos < inodo.tamano; i++) {
            if (bloque.datos[i] != '\0') {
                resultado += bloque.datos[i];
                bytesLeidos++;
            }
        }
        
        bloqueActual = bloque.siguienteBloque;
    }
    
    resultado += "\n----------------------------------------\n";
    resultado += "Bloques usados: " + std::to_string(contadorBloques) + "\n";
    resultado += "Bytes leidos: " + std::to_string(bytesLeidos) + "\n";
    
    archivo.close();
    return resultado;
}

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
    int bloqueActual = 0;
    
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
        bloqueActual = bloquesLibres[i];
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
    bool primero = true;

    // Recorrer los 16 inodos en el Bloque 1 (posicion 256)
    for (int i = 0; i < 16; i++) {
        archivo.seekg(256 + i * sizeof(Inodo));
        archivo.read(reinterpret_cast<char*>(&inodo), sizeof(Inodo));
        
        // Si el inodo está marcado como activo (1)
        if (inodo.activo == 1) {
            if (!primero) {
                listaNombres += ", "; // Separador
            }
            listaNombres += inodo.nombre;
            primero = false;
        }
    }

    archivo.close();
    
    if (listaNombres.empty()) {
        return "No hay figuras guardadas.";
    }

    return listaNombres;
}