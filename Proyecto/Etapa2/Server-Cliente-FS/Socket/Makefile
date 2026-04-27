# VARIABLES
XC=c++
STD=-std=c++20
WFLAGS=-Wall -Wextra
LIBS=-lssl -lcrypto

# DIRECTORIOS
SRC=src
BUILD=build

# ARCHIVOS
DIR=$(shell find -L $(SRC) -type d)
FHEADER=$(wildcard $(DIR:%=%/*.h))
FSOURCE=$(wildcard $(DIR:%=%/*.cc))
FOBJECT=$(FSOURCE:$(SRC)/%.cc=$(BUILD)/%.o)

.PHONY: all clean

all: $(FOBJECT)

$(BUILD)/%.o: $(SRC)/%.cc | $(BUILD)
	$(XC) $(STD) $(WFLAG) -c -g $^ -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)

show:
	echo $(FSOURCE) $(FHEADER) $(FOBJECT)
