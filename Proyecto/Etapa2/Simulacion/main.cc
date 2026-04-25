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

// Todos los componentes llaman esto en lugar de cout directo
void logger(const string& componente, const string& mensaje) {
    lock_guard<mutex> lock(console_mtx);
    cout << "[" << componente << "]: " << mensaje << "\n";
}

int main() {
    const int NUM_CLIENTES = 3;

    cout << "=== Simulacion Protocolo Grupal (Etapa 2) ===\n\n";
    cout << "Escenario: flujo normal + P/K/ (muerte del servidor)\n\n";

    // Cada cliente tiene su propia cola de solicitudes hacia el Tenedor
    // para evitar el filtrado por id y el busy-wait que causa bad_alloc
    vector<Cola*> colas_entrada_tenedor;
    for (int i = 0; i < NUM_CLIENTES; i++) {
        colas_entrada_tenedor.push_back(new Cola());
    }

    // Canal bidireccional entre Tenedor y Servidor
    Cola cola_tenedor_servidor;
    Cola cola_servidor_tenedor;

    // Cada cliente tiene su propia cola de respuesta para que el Tenedor
    // pueda responderle directamente sin que otro cliente lea su mensaje
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

    // Hilo aparte que mata al servidor después de 600ms para simular caída
    // mientras algunos clientes todavía están en medio de su sesión
    thread hilo_kill([&srv]() {
        this_thread::sleep_for(chrono::milliseconds(600));
        srv.matar();
    });

    // Esperar en orden: primero clientes, luego Tenedor, luego Servidor
    // Si se hace al revés puede haber deadlock porque el Tenedor espera
    // que todos los clientes terminen antes de cerrar
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
