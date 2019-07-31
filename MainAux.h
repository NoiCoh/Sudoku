#ifndef MAINAUX_H
#define MAINAUX_H
#include "Solver.h"

void printErrorMode();
Board* createDefaultBoard();
Game* initializeGame();
void UpdateGame(Game* game, Board *userBoard, enum modes mode);
void printWelcome();
void printNTimes(int n);

/**
 * Prints the current state of @param board. if the markErrors parameter is true-
 cells that participate in an error should be marked .
 */
void printBoard(Game* game);

/**
 * Make a deep copy of @param board to @param copyBorad.
 */
void makeCopyBoard(Board* borad, Board* copyBorad);

/**
 * If a standard failure or memory error occurs (failure of malloc, scanf, etc.)
 * the program prints "Error: <function-name> has failed" and exits.
 */
void funcFailed(char* str);

/**
 *check if board has a cell that marked as an error.
 */
bool isBoardErroneous(Board* board);

/**
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF();


/*
* set value in board's game
*/
void setValue(Game* game, int col, int row, int value) {
	game->board->cells[row][col].value = value;
}

/*
Count how many empty cells in board's game
**/
int FindHowMuchEmptyCells(Game* game);

#endif
