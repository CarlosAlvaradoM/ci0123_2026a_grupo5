// Copyright 2026a Equipo 5 CI-0123. ECCI-UCR. CC BY 4.0

#ifndef LLENARFIGURAS_H
#define LLENARFIGURAS_H

/**
 * @brief Llama a la funcion de llenar figuras en figuras.dat
 * 
 * Esta función llama varias veces a escribirStringEnBloquesLibres
 * para ahorrar trabajo en el servidor
 * 
 * @param ruta Ruta del archivo del sistema de archivos
 */
void ponerFiguras(const char* ruta);

#endif /* LLENARFIGURAS_H */