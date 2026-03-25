CXX := g++
CXXFLAGS := -g -c
LDFLAGS := -g
LDLIBS := -lssl -lcrypto

DEPS := Socket.h VSocket.h SSLsocket.h
TARGET := cliente

OBJS := main.o Socket.o VSocket.o SSLsocket.o Menu.o Parser.o

all: $(TARGET)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

run-ssl: $(TARGET)
	./$(TARGET) --ssl

clean:
	@rm -f *.o *.out $(TARGET)

.PHONY: all clean run run-ssl