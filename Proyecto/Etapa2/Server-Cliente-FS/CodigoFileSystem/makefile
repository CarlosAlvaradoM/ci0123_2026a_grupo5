# Nombre del ejecutable
TARGET = programa

# Lista de archivos fuente
SRCS = main.cc CrearDat.cc BitMap.cc Inodos.cc Bloques.cc ManipularDat.cc

# Regla principal (se ejecuta al escribir 'make')
$(TARGET): $(SRCS)
	g++ -o $(TARGET) $(SRCS)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Regla para limpiar archivos generados
clean:
	rm -f $(TARGET) figuras.dat