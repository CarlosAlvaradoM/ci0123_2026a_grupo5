#ifndef MENU_H
#define MENU_H

void mostrarMenu();
int elegirFigura();
int elegirParte();
void construirRequest(char* buffer, const char* figura, int parte);

const char* obtenerFigura(int index);

#endif