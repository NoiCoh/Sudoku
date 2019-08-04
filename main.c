#include <stdio.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include "ILPsolver.h"


int main() {
	Game* game;
	int N,res;
	game = initializeGame();
	game->mode = solve;
	restart();
	//game->board = getUserBoard(game,"C:\\Users\\Noi\\Desktop\\game.txt");
	//N = game->board->blocksize.m * game->board->blocksize.n;
	//res=exhaustiveBacktracking(game, N);
	//printf("%d",res);
	//return res;
	return 0;
}
