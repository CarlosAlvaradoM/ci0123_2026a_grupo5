CXX := g++
CXXFLAGS := -g -c
LDFLAGS := -g

DEPS := Socket.h VSocket.h
TARGET := cliente

OBJS := main.o Socket.o VSocket.o

all: $(TARGET)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	@rm -f *.o *.out $(TARGET)

.PHONY: all clean run