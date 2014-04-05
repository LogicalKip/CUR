#Makefile pour le calculateur Urban Rivals
#Intended to be at the root of the project folder

EXE=prgm
CC=gcc

SRC_DIR=src
OBJ_DIR=obj
INCLUDE_DIR=include

_FICHIERS_H = autresFonctions.h calculPrealable.h constantes.h fonctionsAnnexes.h fonctionsBoucles.h lectureFichiers.h main.h
FICHIERS_H = $(patsubst %, $(INCLUDE_DIR)/%, $(_FICHIERS_H))

_OBJ = autresFonctions.o calculPrealable.o fonctionsAnnexes.o fonctionsBoucles.o lectureFichiers.o main.o
OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))


FLAGS=-Wall -Wextra



CUR: $(OBJ)
	$(CC) -o $(EXE) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(FICHIERS_H)
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	rm $(OBJ) $(EXE) 
