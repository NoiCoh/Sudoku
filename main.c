#include <stdio.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include "ILPsolver.h"


int main() {
	Game* game;
	game = initializeGame();
	game->mode = solve;
	restart();
	return 0;
}
