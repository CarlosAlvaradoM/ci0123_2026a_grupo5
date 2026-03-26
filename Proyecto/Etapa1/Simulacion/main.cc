/**
 * @file main.cc
 * @brief Programa principal para simulación del protocolo CI-0123
 * 
 * Este programa implementa una simulación del protocolo de comunicación
 * para consulta de piezas LEGO. Utiliza una arquitectura con cliente,
 * tenedor es decir intermediario y servidor de piezas.
 * 
 * La simulación ejecuta un cliente que realiza una secuencia de comandos
 * contra el tenedor para verificar el correcto funcionamiento del protocolo.
 * 
 * @author Kiara Brenes
 * @date 2026
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include "Cliente.h"

using namespace std;

mutex console_mtx;  ///< Mutex para sincronizar la salida por consola

/**
 * @brief Función de logging sincronizada para salida por consola
 * @param componente Nombre del componente que genera el mensaje (CLIENTE, TENEDOR, HILO_TRABAJADOR)
 * @param mensaje Mensaje a mostrar
 * 
 * Utiliza un mutex global para garantizar que los mensajes de diferentes
 * hilos no se mezclen en la salida estándar.
 */
void logger(const string& componente, const string& mensaje) {
    lock_guard<mutex> lock(console_mtx);
    cout << "[" << componente << "]: " << mensaje << endl;
}

/**
 * @brief Función principal del programa
 * @return 0 si la ejecución es exitosa
 * 
 * Crea una instancia del tenedor y un cliente con ID 12, luego ejecuta
 * el cliente en un hilo separado para simular el flujo de comunicación.
 */
int main() {
    cout << "Simulación de protocolo(con una isla)" << endl;

    Tenedor tenedor;
    Cliente cliente(tenedor, 12);

    thread hilo(&Cliente::ejecutar, &cliente);
    hilo.join();

    cout << "Fin de la simulación" << endl;
    return 0;
}