# SERVER-CLIENTE-FS

Sistema cliente-servidor para gestión de figuras de Lego almacenadas en un sistema de archivos virtual (FS). El servidor expone un API HTTP simple y el cliente se conecta mediante sockets TCP/IP para consultar y recibir las figuras.

## Descripción del diseño

### Arquitectura general

El proyecto se organiza en tres módulos principales:

```
SERVER-CLIENTE-FS/
├── Server-FS/      # Servidor HTTP + FileSystem
├── Cliente/        # Cliente interactivo
├── Socket/         # Capa de abstracción de sockets
└── Common/         # Componentes compartidos
```

### Componentes del sistema

#### 1. **Socket** (capa de transporte)
- **Socket.cc/h**: Implementa sockets TCP/IP con soporte IPv4/IPv6
- **VSocket.cc/h**: Clase base virtual para abstracción de sockets
- Proporciona métodos: `Connect()`, `Bind()`, `Accept()`, `Read()`, `Write()`, `Close()`

#### 2. **Server-FS** (servidor HTTP + FileSystem)

**CodigoFileSystem/** - Módulo de almacenamiento:
- `CrearDat.cc/h`: Crea archivo de 256KB inicializado en ceros
- `BitMap.cc/h`: Gestiona el bloque 0 como bitmap de 1024 bits
- `Inodos.cc/h`: Maneja la tabla de 16 inodos en el bloque 1 (16 bytes cada uno)
- `Bloques.cc/h`: Limpia bloques de datos del 2 al 1023
- `ManipularDat.cc/h`: Operaciones de lectura/escritura de figuras
- `LlenarFiguras.cc/h`: Precarga figuras predefinidas en el FS

**ForkMirrorServer.cc** - Servidor principal:
- Escucha en puerto 2026 usando sockets IPv6/IPv4
- Acepta conexiones y hace fork() por cada cliente
- Implementa protocolo HTTP simple (GET /figura)
- Procesa peticiones y responde con datos del FS

#### 3. **Cliente** (aplicación interactiva)
- `Cliente.cc/h`: Lógica del cliente (conexión, requests, UI)
- `Main.cc`: Punto de entrada, procesa argumentos de línea de comandos
- Características:
  - Selección de protocolo (IPv4/IPv6)
  - Opción SSL
  - Menú interactivo para elegir figuras
  - Muestra las piezas recibidas del servidor

### Flujo de datos

```bash
┌─────────┐     HTTP Request       ┌─────────┐     FS Operations     ┌──────────┐
│ Cliente │ ──────────────────────>│ Server  │ ─────────────────────>│ Archivo  │
│         │  GET /jirafa HTTP/1.1  │         │  leerFigura()         │ .dat     │
│         │                        │         │  ListData()           │ 256KB    │
│         │ <──────────────────────│         │ <─────────────────────│          │
│         │   HTTP/1.1 200 OK      │         │   Datos de figura     │          │
│         │   Figuras en texto     │         │                       │          │
└─────────┘                        └─────────┘                       └──────────┘
```

### Estructura del FileSystem (figuras.dat)

``` bash
Tamaño total: 256KB (262,144 bytes)

┌─────────────────────────────────────────────────────────────────┐
│ Bloque 0       │ Bloque 1        │ Bloques 2-1023               │
│ (0-255 bytes)  │ (256-511 bytes) │ (512-262,143 bytes)          │
├────────────────┼─────────────────┼──────────────────────────────┤
│ BITMAP         │ TABLA INODOS    │ BLOQUES DE DATOS             │
│ 128 bytes      │ 16 inodos       │ Por bloque:                  │
│ 1024 bits      │ 16 bytes/cada   │ - 240 bytes de datos         │
│                │                 │ - 2 bytes puntero sig. bloque│
└────────────────┴─────────────────┴──────────────────────────────┘

Inodo (16 bytes):
├── nombre[8] -----------> Nombre de la figura (máx 7 caracteres)
├── tamano(4) -----------> Tamaño total en bytes
├── primerBloque(2) -----> Número del primer bloque de datos
├── activo(1) -----------> 1=activo, 0=libre
└── reservado(1) --------> Alineación

Lista enlazada de bloques:
[Bloque n] → siguienteBloque = m → [Bloque m] → siguienteBloque = 65535 (EOF)
```

### Protocolo de comunicación

El servidor implementa un subconjunto minimalista de HTTP/1.1:

**Request**:
```
GET /[nombre_figura] HTTP/1.1\r\n\r\n
GET / HTTP/1.1\r\n\r\n
```

**Response**:
```
HTTP/1.1 200 OK\r\n
Content-Type: text/plain\r\n\r\n
[contenido de la figura o lista de figuras]

HTTP/1.1 404 Not Found\r\n
Content-Type: text/plain\r\n\r\n
Figura no encontrada
```

### Manejo de concurrencia

El servidor utiliza `fork()` para manejar múltiples clientes:
1. Proceso principal acepta conexiones en el socket pasivo
2. Por cada conexión, crea un hijo con `fork()`
3. El hijo procesa la solicitud y termina
4. El padre continúa escuchando nuevas conexiones

## Requisitos previos

- Sistema operativo Linux/Unix
- Compilador con soporte C++11 o superior
- Make

## Instalación y compilación

### 1. Compilar el servidor

```bash
cd Server-FS
make clean
make
```

### 2. Compilar el cliente

```bash
cd Cliente
make clean
make
```

## Ejecución

### 1. Iniciar el servidor

```bash
cd Server-FS/bin
./server.out
```

El servidor comenzará a escuchar en el puerto 2026 en todas las interfaces (IPv4 e IPv6).

### 2. Ejecutar el cliente (en otra terminal)

```bash
cd Cliente/bin
./cliente.out <IP_SERVIDOR> <PUERTO>
```

**Parámetros**:
- `<IP_SERVIDOR>`: Dirección IP del equipo en uso (IPv4 o IPv6) restricciones 127.0.0.1 y localhost.
- `<PUERTO>`: Puerto del servidor (debe ser 2026, el mismo configurado en el servidor)

**Ejemplo**:
```bash
./cliente.out 192.168.7.9 2026
./cliente.out fd00::2077:5d40:96f0:7f21 2026
```

### 3. Interacción con el cliente

Al ejecutar el cliente, se solicitará:

1. **Protocolo**:
   - `1` = IPv4
   - `2` = IPv6

2. **SSL**:
   - `1` = Sí
   - `2` = No

3. **Selección de figura**:
   - Se muestra lista numerada de figuras disponibles
   - Ingresar el número correspondiente
   - Opción `9` o último número = Salir del programa

## Ejemplo completo de ejecución

### Terminal 1 - Servidor:

```bash
user@user:~/Server-Cliente-FS/Server-FS/bin$ ./server.out 
Request recibido:
GET / HTTP/1.1


Request recibido:
GET /jirafa HTTP/1.1


Request recibido:
GET /gotica HTTP/1.1

```

### Terminal 2 - Cliente:

```bash
user@user:~/Server-Cliente-FS/Cliente/bin$ ./cliente.out 192.168.7.9 2026
Que protocolo desea usar? (ingrese el numero):
1. IPv4
2. IPv6
1
Desea usar SSL para conectarse? (ingrese el numero):
1. Si
2. No
2
Que figura desea consultar (ingrese el numero):
1. caballo
2. elefant
3. jirafa
4. ovejaB
5. ovejaN
6. pato
7. pez
8. gotica
9. Salir del programa
3
Piezas para armar jirafa:
Leyendo figura: jirafa
----------------------------------------
4 brick 2x2 black
4 brick 2x2 yellow
6 brick 2x1 yellow
6 brick 2x1 black
1 brick 2x6 yellow
2 brick 2x4 yellow
1 brick 2x3 yellow
1 brick 1x4 yellow
1 brick 2x3 red
2 brick 1x1 eye yellow
----------------------------------------
Bloques usados: 1
Bytes leidos: 188

Que figura desea consultar (ingrese el numero):
1. caballo
2. elefant
3. jirafa
4. ovejaB
5. ovejaN
6. pato
7. pez
8. gotica
9. Salir del programa
8
Piezas para armar gotica:
Leyendo figura: gotica
----------------------------------------
12 brick 2x4 light_gray base
8 brick 2x10 dark_gray foundation
24 brick 1x2 light_gray main_wall
16 brick 1x4 light_gray side_wall
4 slope 2x2 45_deg dark_gray roof
8 slope 1x2 45_deg dark_gray roof_edge
2 cone 1x1 light_gray spire_top
6 cylinder 1x1 trans_blue window
1 arch 1x6x2 light_gray main_entrance
4 plate 4x6 dark_gray floor_section
2 door 1x4x6 brown main_gate
10 tile 1x1 white floor_pattern
10 tile 1x1 black floor_pattern
8 brick 1x1x5 light_gray pillar
4 modified_brick 1x1_with_stud light_gray
12 technic_brick 1x2 light_gray support
2 dish 2x2 trans_clear rose_window
16 brick 2x2 dark_gray buttress
8 plate 1x1 gold decorative_trim
1 flag 2x2 blue kingdom_banner
1 fence 1x4x2 black garden_wall
4 slope_inverted 2x2 light_gray cornice
6 brick 1x1 trans_yellow altar_light
2 ladder 1x2x10 silver bell_tower
1 bell 2x2 gold tower_interior
12 brick 1x3 light_gray clerestory
4 wedge_plate 4x4 light_gray corner
8 brick 1x1 light_gray crenelation
2 hinge_brick 1x2 light_gray transept
1 baseplate 16x16 green landscape
----------------------------------------
Bloques usados: 5
Bytes leidos: 1032

Que figura desea consultar (ingrese el numero):
1. caballo
2. elefant
3. jirafa
4. ovejaB
5. ovejaN
6. pato
7. pez
8. gotica
9. Salir del programa
9
```

## Estructura de archivos compilados

```
Server-FS/
├── bin/
│   └── server.out
└── obj/

Cliente/
├── bin/
│   └── cliente.out
└── obj/
```

## Notas

- El archivo `figuras.dat` se crea automáticamente en `Server-FS/src/CodigoFileSystem/` si no existe al iniciar el servidor
- Las figuras precargadas incluyen: caballo, elefant, jirafa, ovejaB, ovejaN, pato, pez, gotica
- El servidor usa `fork()` por cada cliente, por lo que puede manejar múltiples conexiones simultáneas
- El cliente funciona en modo interactivo y termina al seleccionar la opción de salida

## Autores

- Equipo 5 - CI-0123
- Carlos Alvarado
- Kiara Brenes
- Juan Gauthier
- ECCI, Universidad de Costa Rica
- 2026a
