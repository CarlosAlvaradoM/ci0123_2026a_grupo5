# Cliente Figuras LEGO

Cliente en C++ para consultar piezas LEGO desde un servidor web, con soporte para **IPv4** e **IPv6**, así como **HTTP** y **HTTPS** (SSL/TLS).

---

## Requisitos

- **Sistema operativo:** Linux/Unix
- **Compilador:** g++ con soporte C++11 o superior
- **Bibliotecas:**
  - OpenSSL (`libssl-dev`, `libcrypto-dev`)

### Instalación de dependencias (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install g++ libssl-dev
```

---

## Compilación

El proyecto incluye un `Makefile` para facilitar la compilación:

```bash
make
```

Esto generará el ejecutable `cliente`.

---

## Ejecución

### Modo HTTP (sin SSL)

```bash
make run
```

O directamente:

```bash
./cliente
```

### Modo HTTPS (con SSL)

```bash
make run-ssl
```

O con el argumento:

```bash
./cliente --ssl
```

---

## Flujo del programa

1. **Selección de protocolo**  
   El programa solicita elegir entre IPv4 o IPv6.  
   Si se elige IPv6, verifica automáticamente la conectividad antes de continuar.

2. **Obtención de figuras**  
   Descarga la lista de figuras disponibles desde el servidor (`/lego/figuras.php`).  
   Si falla la conexión, utiliza una lista estática predefinida.

3. **Menú interactivo**  
   Muestra las figuras disponibles y solicita al usuario:
   - Seleccionar una figura (por número)
   - Elegir la parte de la figura (1 o 2)

4. **Consulta al servidor**  
   Construye y envía la solicitud HTTP/HTTPS correspondiente.

5. **Resultados**  
   Muestra las piezas LEGO requeridas y el total de piezas.

---

## IPv6

Para usar IPv6 es necesario estar conectado a la **red interna de la ECCI** o mediante **VPN**.  
El programa verifica la conectividad antes de continuar con IPv6.

---

## Estructura del código

| Archivo | Descripción |
|--------|------------|
| `main.cc` | Punto de entrada, manejo de argumentos y flujo principal |
| `Menu.cc/h` | Menú interactivo, selección de figura/parte y solicitudes HTTP |
| `Parser.cc/h` | Extracción del cuerpo HTML y parseo de piezas LEGO |
| `VSocket.cc/h` | Clase base abstracta para sockets |
| `Socket.cc/h` | Implementación de sockets TCP (IPv4/IPv6) |
| `SSLsocket.cc/h` | Implementación de sockets seguros con OpenSSL |

---

## 🔧 Comandos Makefile útiles

| Comando | Descripción |
|--------|------------|
| `make` | Compila el proyecto |
| `make run` | Ejecuta el cliente (HTTP) |
| `make run-ssl` | Ejecuta el cliente con SSL |
| `make clean` | Limpia archivos objeto y ejecutable |

---

## Notas

- El servidor utilizado es `os.ecci.ucr.ac.cr` (IPv4: `163.178.104.62`).
- Para IPv6 se usa la dirección `fe80::8f5a:e2e1:7256:ffe3` con interfaz `enp0s31f6`.
- Si la lista de figuras no se obtiene del servidor, se utiliza una lista estática por defecto.

---

## 👥 Autores

- **Carlos Alvarado**
- **Kiara Brenes**

**Universidad de Costa Rica**  
ECCI - CI0123 Proyecto integrador de redes y sistemas operativos  
2026
