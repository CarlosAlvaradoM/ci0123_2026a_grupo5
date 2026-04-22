# Directorios
SC_DIR := Servidor-Cliente
FS_DIR := CodigoFileSystem

# Compilador
CXX := g++
CXXFLAGS := -g -c -std=c++11 -I$(SC_DIR) -I$(FS_DIR)
LDFLAGS := -pthread

# Objetos
SC_OBJS := $(SC_DIR)/VSocket.o $(SC_DIR)/Socket.o

FS_OBJS := \
$(FS_DIR)/ManipularDat.o \
$(FS_DIR)/CrearDat.o \
$(FS_DIR)/BitMap.o \
$(FS_DIR)/Inodos.o \
$(FS_DIR)/Bloques.o \
$(FS_DIR)/LlenarFiguras.o

TARGETS := ForkMirrorServer.out MirrorClient.out

all: $(TARGETS)

# Regla general
%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

# Servidor-Cliente
$(SC_DIR)/%.o: $(SC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

# FileSystem
$(FS_DIR)/%.o: $(FS_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

# Link
ForkMirrorServer.out: ForkMirrorServer.o $(SC_OBJS) $(FS_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

MirrorClient.out: MirrorClient.o $(SC_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f *.o $(SC_DIR)/*.o $(FS_DIR)/*.o *.out $(FS_DIR)/figuras.dat

.PHONY: all clean