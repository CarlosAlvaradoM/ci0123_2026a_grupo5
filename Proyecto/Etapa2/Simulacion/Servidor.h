#pragma once
#include "Cola.h"
#include <map>
#include <string>
#include <vector>
#include <atomic>

using namespace std;

class Servidor {
private:
    map<string, vector<string>> figuras;
    Cola& entrada;
    Cola& salida;
    atomic<bool> vivo;

public:
    Servidor(Cola& entrada, Cola& salida);
    void matar();
    void correr();
};