#include <string>
#include <iostream>
#include <cstdlib>

#include "Parser.h"

std::string extraerHTML(const std::string& respuestaServer) {
    // Buscar "\r\n\r\n" para final de encabezado HTTP
    size_t posicionSeparacionCuerpo = respuestaServer.find("\r\n\r\n");

    // No hay separador, respuesta mal formada o msj HTTP no estandar
    if (posicionSeparacionCuerpo == std::string::npos) {
        // Devolver respuesta original
        return respuestaServer; 
    }

    // Saltar los 4 caracteres, hasta el final
    return respuestaServer.substr(posicionSeparacionCuerpo + 4);
}

void imprimirBricks(const std::string& contenidoHtml, int parteFigura, const char* nombreFigura) {
    //Parte del string donde se esta buscando
    size_t posicionActual = 0;
    // Suma total de piezas
    int totalPiezas = 0;

    // Encabezado consola
    std::cout << "\nPiezas de lego encontrados\n";
    std::cout << "Figura: " << nombreFigura << " Parte: " << parteFigura << "\n";

    // Termina cuando no hayan filas en la tabla
    while (true) {
        // Buscar etiqueta de apertura de una fila <TR> desde posicionActual
        size_t inicioFila = contenidoHtml.find("<TR>", posicionActual);
        
        // Si no hay se termina
        if (inicioFila == std::string::npos) {
            break;
        }

        // Buscar la etiqueta de cierre de la misma fila
        size_t finFila = contenidoHtml.find("</TR>", inicioFila);
        
        // Si no se encuentra, se sale
        if (finFila == std::string::npos) {
            break;
        }

        // Obtener lo que esta entre cada etiqueta <TR> y </TR>
        std::string textoFila = contenidoHtml.substr(inicioFila, finFila - inicioFila);

        // Si es <TH> o con colspan se salta
        if (textoFila.find("<TH>") != std::string::npos || textoFila.find("COLSPAN") != std::string::npos) {
            // Otra fila
            posicionActual = finFila;
            // Inicio while
            continue;
        }

        // Buscar la primera celda de datos <TD ALIGN=center> que es la cantidad
        size_t inicioCeldaCantPiezas = textoFila.find("<TD ALIGN=center>");
        
        if (inicioCeldaCantPiezas != std::string::npos) {
            // Sumar los 18 caracteres de la etiqueta para estar en el numero
            size_t posicionNumero = inicioCeldaCantPiezas + 18; 
            
            // Cantidad de piezas de str a int
            int cantidadPiezas = atoi(textoFila.c_str() + posicionNumero);

            // Buscar la segunda celda de datos <TD ALIGN=center> que es la descripcion
            size_t inicioCeldaDescripcion = textoFila.find("<TD ALIGN=center>", posicionNumero);
            
            if (inicioCeldaDescripcion != std::string::npos) {
                // Sumar los 18 caracteres de la etiqueta para estar en la descr
                size_t posicionTexto = inicioCeldaDescripcion + 18;
                
                // Buscar donde termina </TD>
                size_t finCeldaDescripcion = textoFila.find("</TD>", posicionTexto);
                
                // Extraer el brick
                std::string descripcionPieza = textoFila.substr(posicionTexto, finCeldaDescripcion - posicionTexto);

                // Revisar cantidad
                if (cantidadPiezas > 0) {
                    std::cout << cantidadPiezas << " piezas de " << descripcionPieza << std::endl;
                    // Contador total
                    totalPiezas += cantidadPiezas;
                }
            }
        }
        // Siguiente fila
        posicionActual = finFila;
    }

    // Total de piezas
    std::cout << "\n" << "Total de piezas: " << totalPiezas << "\n\n";
}