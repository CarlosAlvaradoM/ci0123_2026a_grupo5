CXX = g++
CXXFLAGS = -std=c++17 -pthread
TARGET = simulacion
SRCS = main.cc Servidor.cc Tenedor.cc Cliente.cc

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)