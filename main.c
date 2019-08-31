#include <stdio.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include "ILPsolver.h"


int main() {
	Game* game;
	game = initializeGame();
	restart();
	return 0;
}
