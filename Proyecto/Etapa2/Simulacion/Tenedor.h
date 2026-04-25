#pragma once
#include "Cola.h"
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <map>

using namespace std;

class Tenedor {
private:
    vector<Cola*>& colas_entrada;
    Cola& salida_servidor;
    Cola& entrada_servidor;
    vector<Cola*>& colas_respuesta;
    int num_clientes;

    mutex mtx_servidor;
    atomic<bool> servidor_vivo;
    map<string, int> pendientes;

    void escuchar_servidor();
    void atender_cliente(int id_cliente);

public:
    Tenedor(vector<Cola*>& colas_entrada, Cola& salida_servidor,
            Cola& entrada_servidor, vector<Cola*>& colas_respuesta,
            int num_clientes);
    void correr();
};
