# Linux Makefile
# Socket classes
#

CXX := g++
CXXFLAGS := -g -c
DEPS := Socket.h VSocket.h
TARGETS := ipv4-test.out ipv6-test.out

all: $(TARGETS)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

ipv4-test.out: ipv4-test.o Socket.o VSocket.o
	$(CXX) -g $^ -o $@

ipv6-test.out: ipv6-test.o Socket.o VSocket.o
	$(CXX) -g $^ -o $@

clean:
	@rm -f *.o *.out

.PHONY: all clean