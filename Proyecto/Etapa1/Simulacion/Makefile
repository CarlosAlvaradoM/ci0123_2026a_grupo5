# Configuración del compilador
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra

# Nombre del ejecutable
TARGET = simulacion

# Archivos del proyecto 
SRCS = main.cc Cliente.cc Tenedor.cc Servidor.cc
HEADERS = Cliente.h Tenedor.h Servidor.h
OBJS = $(SRCS:.cc=.o)

# Regla principal (compila y da permisos)
all: $(TARGET)
	@chmod +x $(TARGET)
	@echo "[SISTEMA]: Compilación completada y permisos otorgados a ./$(TARGET)"

# Enlace del binario
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@chmod +x $(TARGET)

# Compilación de archivos objeto (.cc -> .o)
%.o: %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para ejecutar directamente
run: all
	@echo "[SISTEMA]: Iniciando simulación..."
	./$(TARGET)

# Limpieza de archivos temporales y binarios
clean:
	rm -f $(TARGET) $(OBJS)
	@echo "[SISTEMA]: Limpieza profunda realizada."

# Prevenir conflictos con archivos del mismo nombre
.PHONY: all run clean
