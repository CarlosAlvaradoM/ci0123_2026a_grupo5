CXX = g++
CXXFLAGS = -std=c++17 -pthread
TARGET = simulacion
SRCS = main.cc Cliente.cc Tenedor.cc Servidor.cc
HEADERS = Cliente.h Tenedor.h Servidor.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	chmod +x $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
