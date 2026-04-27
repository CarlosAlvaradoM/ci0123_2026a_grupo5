// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef INODOS_H
#define INODOS_H

/**
 * @brief Inicializa el bloque 1 del sistema de archivos como tabla de inodos
 * 
 * Esta función convierte el bloque 1 (bytes 256-511) en una tabla de 16 inodos
 * vacíos. Cada inodo ocupa 16 bytes, totalizando 256 bytes.
 * 
 * @param nombreArchivo Ruta del archivo del sistema de archivos
 *
 * @post El bloque 1 contiene 16 inodos inicializados como "libres"
 * @post Cada inodo tiene nombre vacío, tamaño 0 y estado inactivo
 * 
 * @note 16 inodos × 16 bytes/inodo = 256 bytes (tamaño del bloque 1)
 */
void convertirBloque1AInodo(const char* nombreArchivo);

#endif /* INODOS_H */
