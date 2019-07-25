#ifndef MAINAUX_H
#define MAINAUX_H
#include "Solver.h"
#define SIZE_OF_ROW_COL 9
#define SIZE_OF_BORAD 81


/**
 * Prints the current state of @param board
 */
void printBoard(Board* board);

void printNTimes(int n);

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
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF();
#endif
