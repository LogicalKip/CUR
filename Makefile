#Makefile pour le calculateur Urban Rivals
#Intended to be at the root of the project folder

EXE=urban
CC=clang++-libc++

SRC_DIR=src
OBJ_DIR=obj
INCLUDE_DIR=include

_FICHIERS_H = calculPrealable.hpp fonctionsAnnexes.hpp main.hpp autresFonctions.hpp constantes.hpp fonctionsBoucles.hpp lectureFichiers.hpp

FICHIERS_H = $(patsubst %, $(INCLUDE_DIR)/%, $(_FICHIERS_H))

_OBJ = calculPrealable.o fonctionsBoucles.o autresFonctions.o lectureFichiers.o main.o fonctionsAnnexes.o

OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))


CFLAGS=-g -O3 -Wall -Wextra -std=c++11
LDFLAGS=-std=c++11



CUR: $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXE) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(FICHIERS_H)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ) $(EXE) 
