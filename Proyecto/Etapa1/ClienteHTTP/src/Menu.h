/**
 * @file Menu.h
 * @brief Modulo para el menu para elejir piezas y sus partes
 * 
 * Proporciona funciones para mostrar el menu de figuras LEGO,
 * seleccionar figura y parte, y construir la solicitud HTTP
 * 
 * @author Carlos Alvarado && Kiara Brenes
 * @date 2026
 */

#ifndef MENU_H
#define MENU_H

/**
 * @brief Muestra el menu de figuras disponibles en la consola
 */
void mostrarMenu();

/**
 * @brief Solicita al usuario que seleccione una figura del menu
 * 
 * @return int Indice de la figura seleccionada
 */
int elegirFigura();

/**
 * @brief Solicita al usuario que seleccione una parte de la figura
 * 
 * @return int Numero de parte seleccionado (1 o 2)
 */
int elegirParte();

/**
 * @brief Construye una solicitud HTTP GET para obtener datos de una figura
 *
 * @param buffer Buffer de salida donde se almacenara la solicitud HTTP
 * @param figura Nombre de la figura LEGO
 * @param parte Numero de parte de la figura (1 o 2)
 */
void construirRequest(char* buffer, const char* figura, int parte);

/**
 * @brief Obtiene el nombre de una figura por su indice
 * 
 * @param index Indice de la figura en la lista interna
 * @return const char* Nombre de la figura, o NULL si el indice es invalido
 */
const char* obtenerFigura(int index);

/**
 * @brief Obtiene la lista de figuras desde el servidor
 * 
 * Realiza una solicitud HTTP al servidor para obtener la lista de figuras disponibles.
 * Si la conexion falla, utiliza una lista estatica por defecto.
 * 
 * @param usarIPv6 Indica si se usa IPv6
 * @param usarSSL Indica si se usa SSL
 * @param hostIPv6 Direccion IPv6 opcional
 * @param interfaz Nombre de la interfaz para IPv6
 * @return int Numero de figuras encontradas, -1 si hay error
 */
int obtenerListaFigurasDelServidor(bool usarIPv6, bool usarSSL, const char* hostIPv6 = nullptr, const char* interfaz = nullptr);

/**
 * @brief Libera la memoria de la lista de figuras
 */
void liberarListaFiguras();

#endif