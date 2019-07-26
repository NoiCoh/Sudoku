#ifndef GAME_H
#define GAME_H
#include "MainAux.h"
#include "Parser.h"
#include "ILPsolver.h"

/**
 * initialize board values and params to zero.
 */
Board* initialize(blocksize block);

/**
 * the function returns user board with random fixed cells from the solution board and empty cells.
 * the number of fixed cell is according to the user's input (@param hint).
 */
Board* makeUserBoard(Board* solvedBoard,int hint,blocksize block);

/**
 * set the value z in cell <x,y> (x is the column and y is the row) according to user's command.
 * if the user tries to set a fixed cell, the function prints - "Error: cell is fixed"
 * if the user value is invalid (the value is already in the same box, row or column),
 * the function prints -"Error: value is invalid"
 * if the user set the last empty cell correctly the function prints- "Puzzle solved successfully"
 */
int setCommand(Game *game, char* x, char* y, char* z);

/**
 * check the user value during setCommand.
 * check if the value is invalid (if the value is already in the same box, row or column).
 * if the value is valid returns true, else returns false.
 */
/**bool isValidSet(Board* userBoard,index ind, int val);
**/
/**
 *
 *gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 *hint is given according to the solved board, and it is given even if it is incorrect for the current state of the board.
 *
 */
void hintCommand(Game* game,char* x, char* y);

/**
 *
 *gives a guess hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 *hint is given according to the scores Matrix calculate with LP Solver
 *
 */
void guessHintCommand(Game* game,char* x, char* y);

/**
 * validates that thr current state of the board is solvable.
 * solved the generated board with deterministic backtracking.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
void validate(Board* userBoard,Board* solBoard, blocksize block);

/**
 * the function prints "Exiting..." , frees all memory resources and exits.
 */
void exiting(Board* userBoard,Board* board);

void freeBoard(Board *currentBoard);

void openGame(char* path);

#endif
