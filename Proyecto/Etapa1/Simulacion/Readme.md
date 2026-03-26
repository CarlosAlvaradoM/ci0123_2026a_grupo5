# Simulación de Protocolo CI-0123 - Sistema de Consulta de Piezas LEGO

## Descripción del Proyecto

Este programa simula un protocolo de comunicación para consultar piezas LEGO. Implementa una arquitectura de tres capas:

- **Cliente**: Envía comandos para consultar piezas
- **Tenedor**: Actúa como intermediario procesando las solicitudes del cliente
- **Servidor**: Mantiene el inventario de piezas LEGO

La simulación demuestra cómo establecer sesiones, consultar piezas disponibles, solicitar piezas específicas y manejar tanto solicitudes exitosas como fallidas.

---

## Estructura de Archivos

```
├── main.cc           # Programa principal
├── Cliente.h         # Cabecera de la clase Cliente
├── Cliente.cc        # Implementación de la clase Cliente
├── Tenedor.h         # Cabecera de la clase Tenedor
├── Tenedor.cc        # Implementación de la clase Tenedor
├── Servidor.h        # Cabecera de la clase ServidorPiezas
├── Servidor.cc       # Implementación de la clase ServidorPiezas
└── Makefile          # Archivo de compilación
```

---

## Requisitos del Sistema

- Sistema operativo: Linux, macOS o Windows con WSL
- Compilador: g++ con soporte para C++17
- Biblioteca: pthread (para hilos)

---

## Compilación

### Usando Makefile

Para compilar el programa, ejecute en la terminal:

```bash
make
```

Esto generará el ejecutable `simulacion`.

### Compilación Manual

Si prefiere compilar manualmente:

```bash
g++ -std=c++17 -pthread main.cc Cliente.cc Tenedor.cc Servidor.cc -o simulacion
```

---

## Ejecución

### Ejecutar el programa

```bash
make run
```

o directamente:

```bash
./simulacion
```

### Limpiar archivos compilados

Para eliminar el ejecutable generado:

```bash
make clean
```

---

## Secuencia de Comandos

El cliente ejecuta automáticamente la siguiente secuencia de comandos:

1. **CONNECT** - Establece sesión con el tenedor  
2. **PING** - Verifica conectividad  
3. **STATUS** - Consulta estado del sistema (sesiones activas)  
4. **LISTAR** - Obtiene lista de piezas disponibles  
5. **SOLICITAR brick_2x4** - Solicita una pieza existente  
6. **SOLICITAR pieza_rara** - Solicita una pieza inexistente  
7. **GET brick_1x1** - Obtiene una pieza directamente  
8. **COMANDO_FALSO** - Prueba manejo de comandos inválidos  
9. **DISCONNECT** - Finaliza la sesión  

---

## Salida Esperada

La ejecución mostrará mensajes similares a:

```
Simulación de protocolo (con una isla)
[CLIENTE]: Enviando: CONNECT 12
[TENEDOR]: Recibido: CONNECT 12
[CLIENTE]: Tenedor respondio: OK
[CLIENTE]: Enviando: PING
[TENEDOR]: Recibido: PING
[CLIENTE]: Tenedor respondio: PONG
...
Fin de la simulación
```

---

## Inventario de Piezas

| Pieza | Código Codificado |
|-------|------------------|
| brick_2x4 | GvRWcaqB== |
| placa_2x2 | HzSTdbRC== |
| brick_1x1 | IuPQecSD== |

---

## Comandos Soportados

| Comando | Descripción | Respuesta Esperada |
|---------|-------------|-------------------|
| CONNECT [id] | Establece sesión | OK |
| DISCONNECT [id] | Cierra sesión | OK |
| PING | Verifica conectividad | PONG |
| STATUS | Consulta estado | OK STATUS sesiones_activas=N |
| LISTAR | Lista piezas | OK LISTAR pieza1 pieza2 ... |
| SOLICITAR [pieza] | Solicita pieza | REENVIAR pieza código |
| GET [pieza] | Obtiene pieza | OK código |

---

## Notas Importantes

- El cliente se ejecuta en un hilo separado  
- La salida por consola está sincronizada con mutex para evitar mezcla de mensajes  
- Cada solicitud genera un ID único con formato `req-XXXX`  
- Los códigos de piezas están codificados en Base64  

---

## Solución de Problemas

### Error: "g++: command not found"

Instale el compilador g++:

```bash
# Ubuntu/Debian
sudo apt-get install g++

# CentOS/RHEL
sudo yum install gcc-c++
```

### Error: "pthread library not found"

Asegúrese de incluir el flag `-pthread` durante la compilación.

### Permiso denegado al ejecutar

Si el ejecutable no tiene permisos:

```bash
chmod +x simulacion
```

---

## Autor

Kiara Brenes  
Fecha: 2026
