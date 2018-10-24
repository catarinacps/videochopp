#	-- videochopp --
#
#	Makefile do projeto videochopp
#
#	@param target
#		"all" - compila
#		"clean" - limpa os binários gerados na compilação
#		"redo" - limpa binários e então compila
#		"test" - compila e realiza testes
#
#	Se make não recebe parâmetros, a ação default é all

####################################################################################################
#	Dependências:

SHELL := /bin/bash

####################################################################################################
#	Definições:

#	Diretorios do projeto
INC_DIR := include
OBJ_DIR := bin
OUT_DIR := build
SRC_DIR := src
LIB_DIR := lib

DFLAG := 

#	Flags de compilaçao. Debug para uso no GDB
CXX := g++ -std=c++17
DEBUG := $(if $(DFLAG),-g -fsanitize=address)
CXXFLAGS :=\
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wshadow \
	-Wunreachable-code
OPT := $(if $(DFLAG),-O0,-O3)
LDFLAGS :=\
	$(shell pkg-config --libs opencv)
INC := -I$(INC_DIR) \
	$(shell pkg-config --cflags opencv)

####################################################################################################
#	Arquivos:

#	Fonte da main
MAIN := $(wildcard src/*.cpp)

#	Caminho do arquivo estático final
TARGET := $(patsubst %.cpp, $(OUT_DIR)/%, $(notdir $(MAIN)))

#	Outros arquivos fonte
SRC := $(filter-out $(MAIN), $(shell find $(SRC_DIR) -name '*.cpp'))

#	Objetos a serem criados
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

####################################################################################################
#	Regras:

$(TARGET): $(OUT_DIR)/%: $(SRC_DIR)/%.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $(DEBUG) $(OPT) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o:
	$(CXX) $(CXXFLAGS) $(INC) $(DEBUG) $(OPT) -c -o $@ $(filter %/$*.cpp, $(SRC))

####################################################################################################
#	Alvos:

.DEFAULT_GOAL := all

all: deps $(TARGET)

redo: clean all

deps:
	@scripts/fetch-deps.sh $(patsubst %, '%', $(DEPS))

clean:
	rm -f $(OBJ_DIR)/*.o $(INC_DIR)/*~ $(TARGET) *~ *.o

#	Debug de variaveis da make
print-%:
	@echo $* = $($*)

.PHONY: all clean redo deps
