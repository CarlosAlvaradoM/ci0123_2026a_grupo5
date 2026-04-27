#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include "Cola.h"
#include "Cliente.h"
#include "Tenedor.h"
#include "Servidor.h"

using namespace std;

// Compartidos entre todos los componentes para loggear sin mezclar líneas
mutex console_mtx;
atomic<int> req_counter(0);

void logger(const string& componente, const string& mensaje) {
    lock_guard<mutex> lock(console_mtx);
    cout << "[" << componente << "]: " << mensaje << "\n";
}

int main() {
    const int NUM_CLIENTES = 3;

    cout << "=== Simulacion Protocolo Grupal (Etapa 2) ===\n\n";
    cout << "Escenario: flujo normal + P/K/ (muerte del servidor)\n\n";

    // Cada cliente tiene su propia cola de entrada al Tenedor para que
    // el Tenedor pueda atenderlos con un hilo dedicado sin necesidad de
    // filtrar mensajes por id_cliente
    vector<Cola*> colas_entrada_tenedor;
    for (int i = 0; i < NUM_CLIENTES; i++) {
        colas_entrada_tenedor.push_back(new Cola());
    }

    // Canal bidireccional entre Tenedor y Servidor
    Cola cola_tenedor_servidor;
    Cola cola_servidor_tenedor;

    // Cola de respuesta exclusiva por cliente; el Tenedor deposita aquí
    // la respuesta que viene del Servidor, y el cliente la lee sin competencia
    vector<Cola*> colas_respuesta;
    for (int i = 0; i < NUM_CLIENTES; i++) {
        colas_respuesta.push_back(new Cola());
    }

    Servidor srv(cola_tenedor_servidor, cola_servidor_tenedor);
    Tenedor ten(colas_entrada_tenedor, cola_tenedor_servidor,
                cola_servidor_tenedor, colas_respuesta, NUM_CLIENTES);

    vector<Cliente*> clientes;
    for (int i = 0; i < NUM_CLIENTES; i++) {
        clientes.push_back(new Cliente(i, *colas_entrada_tenedor[i], *colas_respuesta[i]));
    }

    thread hilo_servidor(&Servidor::correr, &srv);
    thread hilo_tenedor(&Tenedor::correr, &ten);

    vector<thread> hilos_clientes;
    for (int i = 0; i < NUM_CLIENTES; i++) {
        hilos_clientes.emplace_back(&Cliente::correr, clientes[i]);
    }

    // Hilo aparte que mata al servidor a los 600 ms para simular una caída
    // mientras algunos clientes todavía están en medio de su sesión
    thread hilo_kill([&srv]() {
        this_thread::sleep_for(chrono::milliseconds(600));
        srv.matar();
    });

    // El orden de join importa: si se espera primero al Tenedor o al Servidor
    // puede haber deadlock porque el Tenedor no termina hasta que todos los
    // clientes cierran sesión
    for (auto& h : hilos_clientes) h.join();
    hilo_tenedor.join();
    hilo_servidor.join();
    hilo_kill.join();

    for (int i = 0; i < NUM_CLIENTES; i++) {
        delete clientes[i];
        delete colas_respuesta[i];
        delete colas_entrada_tenedor[i];
    }

    cout << "\n=== Simulacion finalizada ===\n";
    return 0;
}
