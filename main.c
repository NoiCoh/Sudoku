#include <stdio.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include "ILPsolver.h"


int main() {
/**if (argc>1){
srand(atoi(argv[1]));
}else{
srand(0);
}
    restart();
    return (argc);*/
	Game* game;
	Game* game1;
	game = initializeGame();
	game1 = initializeGame();
	game->board=getUserBoard(game,"C:\\Users\\inbal\\Desktop\\game.txt");
	printBoard(game);
	ILPsolver(game);
	game1->board = game->solBoard;
	printBoard(game1);
}
