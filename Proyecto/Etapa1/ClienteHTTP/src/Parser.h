#ifndef PARSER_H
#define PARSER_H

#include <string>

std::string extraerHTML(const std::string& respuesta);

void imprimirBricks(const std::string& html, int parte, const char* figura);

#endif