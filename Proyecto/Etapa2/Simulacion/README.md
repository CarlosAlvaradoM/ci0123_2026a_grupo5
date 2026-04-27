# Simulación del Protocolo Grupal — Etapa 2

Simulación en C++ de un sistema distribuido de tres capas: clientes que piden figuras LEGO, un intermediario (Tenedor) que enruta las solicitudes, y un servidor (ServidorPiezas) que mantiene el catálogo. La comunicación entre componentes sigue el protocolo definido para el proyecto integrador de CI-0123.

## Estructura del proyecto

```
.
├── Cola.h          # Cola thread-safe y struct Mensaje (infraestructura base)
├── Cliente.h/cc    # Lógica del cliente: pide directorio, elige figura, cierra sesión
├── Tenedor.h/cc    # Proxy: un hilo por cliente + hilo dedicado a escuchar al servidor
├── Servidor.h/cc   # Catálogo de figuras; puede morir abruptamente con P/K/
├── main.cc         # Punto de entrada, creación de hilos y colas
├── Bloques.h       # Utilidad para limpiar bloques del sistema de archivos LEGO-FS
└── Makefile
```

## Cómo compilar

```bash
make
```

Requiere `g++` con soporte para C++17 y pthreads. Si estás en WSL y el binario no corre, revisa que el archivo esté en el filesystem de Linux (no en `/mnt/c/`).

```bash
# Compilación manual si el Makefile falla
g++ -std=c++17 -pthread main.cc Cliente.cc Tenedor.cc Servidor.cc -o simulacion
```

## Cómo ejecutar

```bash
./simulacion
```

La simulación arranca 3 clientes, un Tenedor y un Servidor. A los 600 ms un hilo auxiliar llama a `Servidor::matar()` para simular una caída abrupta mientras algunos clientes todavía están en sesión.

## Escenarios cubiertos

| Situación | Respuesta del sistema |
|---|---|
| Flujo normal | El cliente pide directorio, elige figura, recibe piezas y cierra con P/Q/ |
| Servidor cae antes de la primera respuesta | Tenedor detecta P/K/ y responde P/S/ al cliente |
| Servidor cae con solicitudes en tránsito | Tenedor vacía la tabla de pendientes y responde P/S/ a cada uno |
| Figura no existe | Servidor responde P/D/404 |
| Cierre ordenado | Tenedor envía P/Q/ al Servidor cuando todos los clientes se desconectaron |

## Protocolo (resumen)

Los mensajes siguen el formato `P/<acción>/<cuerpo>`:

| Mensaje | Dirección | Significado |
|---|---|---|
| `P/R/dir` | Cliente → Servidor | Pedir lista de figuras disponibles |
| `P/D/<lista>` | Servidor → Cliente | Respuesta con datos |
| `P/D/404` | Servidor → Cliente | Recurso no encontrado |
| `P/G/<figura>` | Cliente → Servidor | Pedir piezas de una figura |
| `P/Q/` | Cualquiera | Cierre ordenado de sesión |
| `P/A/` | Tenedor → Cliente | ACK al P/Q/ del cliente |
| `P/K/` | Servidor | Muerte abrupta del servidor |
| `P/S/` | Tenedor → Cliente | Servidor no disponible |

## Diseño de hilos

```
main
 ├─ hilo_servidor   → Servidor::correr()
 ├─ hilo_tenedor    → Tenedor::correr()
 │     ├─ hilo_escucha         (escucha respuestas del Servidor)
 │     ├─ hilo_C0-F-S          (atiende al cliente 0)
 │     ├─ hilo_C1-F-S          (atiende al cliente 1)
 │     └─ hilo_C2-F-S          (atiende al cliente 2)
 ├─ hilo_cliente_0  → Cliente::correr()
 ├─ hilo_cliente_1  → Cliente::correr()
 ├─ hilo_cliente_2  → Cliente::correr()
 └─ hilo_kill       (llama srv.matar() a los 600 ms)
```

Cada cliente tiene dos colas exclusivas: una para enviar al Tenedor y otra para recibir respuestas. Esto evita que un cliente lea el mensaje de otro y elimina la necesidad de filtrar por `id_cliente` dentro de la cola.

## Sincronización

- `console_mtx` — mutex global que serializa las escrituras a `stdout` para que los logs no se intercalen.
- `req_counter` — `atomic<int>` que genera IDs únicos para cada solicitud.
- `Cola` — internamente usa `mutex` + `condition_variable`; los consumidores se bloquean hasta que hay algo que leer.
- `Tenedor::mtx_servidor` — protege `servidor_vivo` y la tabla `pendientes` de accesos concurrentes entre los hilos de los clientes y el hilo de escucha.

