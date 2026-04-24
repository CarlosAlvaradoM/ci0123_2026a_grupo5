# VARIABLES
XC=c++
STD=-std=c++20
WFLAGS=-Wall -Wextra
APPNAME?=program
LIBS?=
SOCKETDIR=../Socket
INCLUDE=-I$(SOCKETDIR)/src/
INCLUDEOBJ=$(SOCKETDIR)/build/Socket.o $(SOCKETDIR)/build/VSocket.o

# DIRECTORIOS
SRC=src
BUILD=build
BIN=bin

# ARCHIVOS
DIR=$(shell find -L $(SRC) -type d)
FHEADER=$(wildcard $(DIR:%=%/*.h))
FSOURCE=$(wildcard $(DIR:%=%/*.cc))
FOBJECT=$(FSOURCE:$(SRC)/%.cc=$(BUILD)/%.o)

.PHONY: all clean run

all: $(BIN)/$(APPNAME)

$(BIN)/$(APPNAME): $(FOBJECT) $(INCLUDEOBJ) | $(BIN)
	$(XC) $(STD) $(WFLAGS) -g $^ $(LIBS) -o $@

$(BUILD)/%.o: $(SRC)/%.cc
	@mkdir -p $(dir $@)
	$(XC) $(STD) $(WFLAGS) -c -g $^ $(INCLUDE) -o $@

$(INCLUDEOBJ):
	make -C $(SOCKETDIR)

$(BUILD):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -rf $(BUILD) $(BIN)
	make clean -C $(SOCKETDIR)

run:
	./$(BIN)/$(APPNAME)

show:
	echo $(FOBJECT)
