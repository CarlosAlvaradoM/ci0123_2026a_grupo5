// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef CREARDAT_H
#define CREARDAT_H

/**
 * @brief Crea un archivo binario de 256KB inicializado con ceros
 * 
 * Esta función genera un archivo de 256KB (262144 bytes)
 * 
 * @param nombreArchivo Ruta y nombre del archivo a crear
 * 
 * @warning Si el archivo ya existe, será sobrescrito completamente

 */
void crearDat(const char* nombreArchivo);

#endif /* CREARDAT_H */
