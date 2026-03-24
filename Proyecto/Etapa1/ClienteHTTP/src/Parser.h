/**
 * @file Parser.h
 * @brief Modulo para parsear respuestas HTTP y extraer informacion de piezas LEGO
 */

#ifndef PARSER_H
#define PARSER_H

#include <string>

/**
 * @brief Extrae el cuerpo HTML de una respuesta HTTP
 * 
 * Separa el cuerpo HTML de los encabezados HTTP buscando "\r\n\r\n"
 * 
 * @param respuesta La respuesta HTTP completa incluyendo encabezados y cuerpo
 * @return std::string El cuerpo HTML o la respuesta original
 */
std::string extraerHTML(const std::string& respuesta);

/**
 * @brief Imprime en consola los bricks encontrados
 * 
 * Analiza el cuerpo HTML, extrae y muestra cada pieza con cantidad y descripcion
 * 
 * @param contenidoHtml El contenido HTML que contiene la tabla de piezas LEGO
 * @param parteFigura Primera mitad o segunda mitad
 * @param nombreFigura Nombre de la figura LEGO
 */
void imprimirBricks(const std::string& contenidoHtml, int parteFigura, const char* nombreFigura);

#endif