#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <optional>

using namespace std;

struct Mensaje {
    int id_cliente;
    string contenido;
    string req_id;
};

class Cola {
private:
    queue<Mensaje> datos;
    mutex mtx;
    condition_variable cv;

public:
    void push(const Mensaje& m) {
        unique_lock<mutex> lock(mtx);
        datos.push(m);
        cv.notify_one();
    }

    Mensaje pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]{ return !datos.empty(); });
        Mensaje m = datos.front();
        datos.pop();
        return m;
    }

    optional<Mensaje> pop_timeout(int ms) {
        unique_lock<mutex> lock(mtx);
        bool llego = cv.wait_for(lock, chrono::milliseconds(ms),
                                 [this]{ return !datos.empty(); });
        if (!llego) return nullopt;
        Mensaje m = datos.front();
        datos.pop();
        return m;
    }

    bool vacia() {
        unique_lock<mutex> lock(mtx);
        return datos.empty();
    }
};
