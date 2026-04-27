// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef BITMAP_H
#define BITMAP_H

/**
 * @brief Convierte el bloque 0 de figuras.dat en un bitmap
 * 
 * @param nombreArchivo Ruta del archivo del sistema de archivos
 * 
 * @post El bloque 0 del archivo contiene un bitmap de 128 bytes a 0
 * @post El archivo permanece cerrado después de la operación
 * 
 * @warning La función sobrescribe el contenido existente en el bloque 0
 */
void convertirBloque0ABitmap(const char* nombreArchivo);

#endif /* BITMAP_H */
