// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef BLOQUES_H
#define BLOQUES_H

/**
 * @brief Limpiar poninedo a cero todos los bloques desde la posición 2 hasta el final
 * 
 * Esta función recorre los bloques 2 al 1023 (1024 bloques totales)
 * 
 * @param nombreArchivo Ruta del archivo del sistema de archivos
 * 
 * @pre El archivo debe existir y ser accesible en modo lectura/escritura
 * @post Los bloques 2 al 1023 contienen únicamente ceros
 * @post El bloque 0 y 1 permanecen sin modificaciones
 * 
 * @warning Los datos existentes en esos bloques se perderán permanentemente
 */
void limpiarBloquesDesde2(const char* nombreArchivo);

#endif /* BLOQUES_H */
