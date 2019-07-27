#ifndef MAINAUX_H
#define MAINAUX_H
#include "Solver.h"

void printErrorMode();
void printWelcome();
/**
 * Prints the current state of @param board
 */
void printBoard(Board* board,bool markErrors);

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
 *check if board has cell that mark as erroneous
 */
bool isBoardErroneous(Board* board);

/**
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF();
#endif
