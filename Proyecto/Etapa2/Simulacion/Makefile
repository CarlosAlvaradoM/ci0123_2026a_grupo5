CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
TARGET   = simulacion

SRCS = main.cc Cliente.cc Tenedor.cc Servidor.cc
OBJS = $(SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
