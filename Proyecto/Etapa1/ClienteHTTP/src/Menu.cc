#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include "Menu.h"
#include "Socket.h"
#include "SSLsocket.h"

static std::vector<std::string> figuras;
static bool listaInicializada = false;

static const char* figuras_estaticas[] = {
   "horse",
   "elephant",
   "giraffe",
   "whitesheep",
   "blacksheep",
   "duck",
   "fish"
};

static const int TOTAL_FIGURAS_ESTATICAS = 7;

int obtenerListaFigurasDelServidor(bool usarIPv6, bool usarSSL, const char* hostIPv6, const char* interfaz) {
    VSocket* s = nullptr;
    char buffer[4096];
    char request[512];
    std::string respuesta;
    
    try {
        if (usarSSL) {
            s = new SSLsocket('s', usarIPv6);
            if (usarIPv6 && hostIPv6 != nullptr) {
                char direccionCompleta[256];
                if (interfaz != nullptr) {
                    snprintf(direccionCompleta, sizeof(direccionCompleta), "%s%%%s", hostIPv6, interfaz);
                    s->Connect(direccionCompleta, "443");
                } else {
                    s->Connect(hostIPv6, "443");
                }
            } else if (usarIPv6) {
                s->Connect("fe80::8f5a:e2e1:7256:ffe3%enp0s31f6", "443");
            } else {
                s->Connect("os.ecci.ucr.ac.cr", "443");
            }
        } else {
            s = new Socket('s', usarIPv6);
            if (usarIPv6 && hostIPv6 != nullptr) {
                char direccionCompleta[256];
                if (interfaz != nullptr) {
                    snprintf(direccionCompleta, sizeof(direccionCompleta), "%s%%%s", hostIPv6, interfaz);
                    s->Connect(direccionCompleta, 80);
                } else {
                    s->Connect(hostIPv6, 80);
                }
            } else if (usarIPv6) {
                s->Connect("fe80::8f5a:e2e1:7256:ffe3%enp0s31f6", 80);
            } else {
                s->Connect("163.178.104.62", 80);
            }
        }
        
        sprintf(request, "GET /lego/figuras.php HTTP/1.1\r\nHost: os.ecci.ucr.ac.cr\r\nConnection: close\r\n\r\n");
        s->Write(request);
        
        int bytesRead;
        while ((bytesRead = s->Read(buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            respuesta += buffer;
        }
        
        delete s;
        
        size_t pos = respuesta.find("\r\n\r\n");
        if (pos == std::string::npos) {
            for (int i = 0; i < TOTAL_FIGURAS_ESTATICAS; i++) {
                figuras.push_back(figuras_estaticas[i]);
            }
            listaInicializada = true;
            return TOTAL_FIGURAS_ESTATICAS;
        }
        
        std::string html = respuesta.substr(pos + 4);
        
        figuras.clear();
        
        size_t currentPos = 0;
        while (currentPos < html.length()) {
            size_t hrefPos = html.find("href=", currentPos);
            size_t srcPos = html.find("src=", currentPos);
            size_t findPos = (hrefPos < srcPos) ? hrefPos : srcPos;
            
            if (findPos == std::string::npos) break;
            
            size_t quoteStart = html.find("\"", findPos);
            if (quoteStart == std::string::npos) {
                currentPos = findPos + 1;
                continue;
            }
            
            size_t quoteEnd = html.find("\"", quoteStart + 1);
            if (quoteEnd == std::string::npos) {
                currentPos = quoteStart + 1;
                continue;
            }
            
            std::string valor = html.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            
            if (valor.find(".php") != std::string::npos || valor.find("figure=") != std::string::npos) {
                size_t figurePos = valor.find("figure=");
                if (figurePos != std::string::npos) {
                    size_t startName = figurePos + 7;
                    size_t endName = valor.find("&", startName);
                    if (endName == std::string::npos) {
                        endName = valor.length();
                    }
                    std::string figura = valor.substr(startName, endName - startName);
                    if (!figura.empty() && figura.find("http") == std::string::npos) {
                        // Verificar si ya existe
                        bool existe = false;
                        for (const auto& f : figuras) {
                            if (f == figura) {
                                existe = true;
                                break;
                            }
                        }
                        if (!existe && figura.length() < 50) {
                            figuras.push_back(figura);
                        }
                    }
                }
            }
            
            currentPos = quoteEnd + 1;
        }
        
        if (figuras.empty()) {
            const char* posibles[] = {"horse", "elephant", "giraffe", "whitesheep", "blacksheep", "duck", "fish", "lion", "tiger", "bear", "monkey"};
            for (int i = 0; i < 11; i++) {
                if (html.find(posibles[i]) != std::string::npos) {
                    bool existe = false;
                    for (const auto& f : figuras) {
                        if (f == posibles[i]) {
                            existe = true;
                            break;
                        }
                    }
                    if (!existe) {
                        figuras.push_back(posibles[i]);
                    }
                }
            }
        }
        
        if (figuras.empty()) {
            for (int i = 0; i < TOTAL_FIGURAS_ESTATICAS; i++) {
                figuras.push_back(figuras_estaticas[i]);
            }
        }
        
        listaInicializada = true;
        return figuras.size();
        
    } catch (const std::exception& e) {
        // Si hay error, usar lista estatica
        if (s) delete s;
        figuras.clear();
        for (int i = 0; i < TOTAL_FIGURAS_ESTATICAS; i++) {
            figuras.push_back(figuras_estaticas[i]);
        }
        listaInicializada = true;
        return TOTAL_FIGURAS_ESTATICAS;
    } catch (...) {
        if (s) delete s;
        figuras.clear();
        for (int i = 0; i < TOTAL_FIGURAS_ESTATICAS; i++) {
            figuras.push_back(figuras_estaticas[i]);
        }
        listaInicializada = true;
        return TOTAL_FIGURAS_ESTATICAS;
    }
}

void liberarListaFiguras() {
    figuras.clear();
    listaInicializada = false;
}

void mostrarMenu() {
    printf("   Menu de figuras disponibles\n");
    for (size_t i = 0; i < figuras.size(); i++) {
        printf("   %zu. %s\n", i + 1, figuras[i].c_str());
    }
}

int elegirFigura() {
    int opcion;
    while (1) {
        printf("\nSeleccione una figura (1-%zu): ", figuras.size());
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            continue;
        }
        if (opcion >= 1 && opcion <= (int)figuras.size()) {
            break;
        }
        printf("Opcion invalida. Debes elegir un numero entre 1 y %zu.\n", figuras.size());
    }
    return opcion - 1;
}

int elegirParte() {
    int parte;
    while (1) {
        printf("Seleccione la parte (1 o 2): ");
        if (scanf("%d", &parte) != 1) {
            while (getchar() != '\n');
            printf("Entrada invalida. Por favor ingrese 1 o 2.\n");
            continue;
        }
        if (parte == 1 || parte == 2) {
            break;
        }
        printf("Opcion invalida. Debes elegir un numero entre 1 y 2.\n");
    }
    return parte;
}

const char* obtenerFigura(int index) {
    if (index >= 0 && index < (int)figuras.size()) {
        return figuras[index].c_str();
    }
    return nullptr;
}

void construirRequest(char* buffer, const char* figura, int parte) {
    sprintf(buffer,
        "GET /lego/list.php?figure=%s&part=%d HTTP/1.1\r\n"
        "Host: os.ecci.ucr.ac.cr\r\n"
        "Connection: close\r\n"
        "\r\n",
        figura, parte);
}