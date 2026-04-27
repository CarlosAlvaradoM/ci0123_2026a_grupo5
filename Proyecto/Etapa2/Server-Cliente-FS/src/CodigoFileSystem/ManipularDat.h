// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef MANIPULARDAT_H
#define MANIPULARDAT_H

#include <string>

/**
 * @brief Lee una figura del sistema de archivos
 * 
 * Busca un inodo con el nombre especificado y recupera todos los datos
 * de la figura, recorriendo la lista enlazada de bloques.
 * 
 * @param nombreArchivo Ruta del archivo del sistema de archivos
 * @param nombreFigura Nombre de la figura a leer (máx. 7 caracteres)
 * @return std::string Contenido de la figura con formato o mensaje de error
 */
std::string leerFigura(const char* nombreArchivo, const char* nombreFigura);

/**
 * @brief Escribe una figura en bloques libres del sistema de archivos
 * 
 * Busca bloques libres usando el bitmap, asigna espacio, crea un inodo
 * y escribe los datos en una estructura de lista enlazada de bloques.
 * 
 * @param nombreArchivo Ruta del archivo del sistema de archivos
 * @param nombreFigura Nombre de la figura a crear (máx. 7 caracteres)
 * @param string Contenido de la figura a escribir
 * 
 */
void escribirStringEnBloquesLibres(const char* nombreArchivo, const char* nombreFigura, const char* string);

/**
 * @brief Lista todas las figuras almacenadas en el sistema de archivos
 * 
 * Recorre la tabla de inodos en el bloque 1 y devuelve los nombres
 * de todas las figuras que están marcadas como activas.
 * 
 * @param nombreArchivo Ruta del archivo del sistema de archivos
 * @return std::string Lista de nombres de figuras (uno por línea)
 */
std::string ListData(const char* nombreArchivo);

#endif /* MANIPULARDAT_H */
