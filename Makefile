CC=g++
CFLAGS= -std=c++11
CMD=${CC} ${FLAGS}

TARGET_EXEC := jogos.out
TAREGT_INSTALL := recursos.tar.gz

SRC_DIR = ./src
INCLUDE_DIR = ./include
BUILD_DIR = ./build
BIN_DIR = ./build/bin
OBJ_DIR = ./build/obj

PATH_SFML = /usr/lib/x86_64-linux-gnu
LIB_SFML =  -lsfml-graphics -lsfml-window -lsfml-system
ALFLAGS = -L $(PATH_SFML) $(LIB_SFML)

all: main

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/sistema.o $(OBJ_DIR)/midia.o $(OBJ_DIR)/tabuleiro.o $(OBJ_DIR)/cadastro.o $(OBJ_DIR)/menu.o
	$(CMD) $(OBJ_DIR)/main.o $(OBJ_DIR)/sistema.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/cadastro.o $(OBJ_DIR)/midia.o $(OBJ_DIR)/tabuleiro.o -o $(BIN_DIR)/$(TARGET_EXEC) $(ALFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/sistema.hpp
	$(CMD) -c $(SRC_DIR)/main.cpp -I$(INCLUDE_DIR) -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/sistema.o: $(SRC_DIR)/sistema.cpp $(INCLUDE_DIR)/tabuleiro.hpp $(INCLUDE_DIR)/midia.hpp
	$(CMD) -c $(SRC_DIR)/sistema.cpp -I$(INCLUDE_DIR) -o $(OBJ_DIR)/sistema.o

$(OBJ_DIR)/midia.o: $(SRC_DIR)/midia.cpp $(INCLUDE_DIR)/midia.hpp
	$(CMD) -c $(SRC_DIR)/midia.cpp -I$(INCLUDE_DIR) -o $(OBJ_DIR)/midia.o

$(OBJ_DIR)/tabuleiro.o: $(SRC_DIR)/tabuleiro.cpp $(INCLUDE_DIR)/tabuleiro.hpp
	$(CMD) -c $(SRC_DIR)/tabuleiro.cpp -I$(INCLUDE_DIR) -o $(OBJ_DIR)/tabuleiro.o


$(OBJ_DIR)/cadastro.o: $(SRC_DIR)/cadastro.cpp $(INCLUDE_DIR)/cadastro.hpp
	$(CMD) -c $(SRC_DIR)/cadastro.cpp -I$(INCLUDE_DIR) -o $(OBJ_DIR)/cadastro.o

$(OBJ_DIR)/menu.o: $(SRC_DIR)/menu.cpp $(INCLUDE_DIR)/menu.hpp $(INCLUDE_DIR)/cadastro.hpp
	$(CMD) -c $(SRC_DIR)/menu.cpp -I$(INCLUDE_DIR) -o $(OBJ_DIR)/menu.o

.PHONY: clean install
clean:
	rm -r $(OBJ_DIR)/*.o $(BIN_DIR)/$(TARGET_EXEC)

install:
	tar xf $(TAREGT_INSTALL) -C $(BUILD_DIR)

