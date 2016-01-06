#Makefile pour le calculateur Urban Rivals
#Intended to be at the root of the project folder

EXE=urban
CC=g++-4.9

SRC_DIR=src
OBJ_DIR=obj
INCLUDE_DIR=include

_FICHIERS_H = allocator.hpp calculPrealable.hpp fonctionsAnnexes.hpp game.hpp main.hpp node.hpp statistics.hpp test_mcts_two_players.hpp autresFonctions.hpp constantes.hpp fonctionsBoucles.hpp mcts.hpp omp_util.hpp test_allocator.hpp bits.hpp display_node.hpp learning.hpp mcts_two_players.hpp openings.hpp test_bits.hpp fast_log.hpp lectureFichiers.hpp monte_carlo.hpp pow.hpp test_fast_log.hpp urban-mcts.hpp

FICHIERS_H = $(patsubst %, $(INCLUDE_DIR)/%, $(_FICHIERS_H))

_OBJ = allocator.o calculPrealable.o fonctionsBoucles.o monte_carlo.o statistics.o test_mcts_two_players.o autresFonctions.o display_node.o lectureFichiers.o node.o test_allocator.o bits.o fast_log.o main.o omp_util.o test_bits.o urban-mcts.o fonctionsAnnexes.o mcts_two_players.o openings.o test_fast_log.o

OBJ = $(patsubst %, $(OBJ_DIR)/%, $(_OBJ))


CFLAGS=-g -O3 -ffast-math -fopenmp -Wall -Wextra -std=c++11
LDFLAGS=-fopenmp -std=c++11



CUR: $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXE) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(FICHIERS_H)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm $(OBJ) $(EXE) 
