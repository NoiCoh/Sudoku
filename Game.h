#ifndef GAME_H
#define GAME_H
#include "Solver.h"
#include "Parser.h"
#include "ILPsolver.h"

/**
 * the function returns user board with random fixed cells from the solution board and empty cells.
 * the number of fixed cell is according to the user's input (@param hint).
 */
Board* makeUserBoard(Board* solvedBoard, int hint, blocksize block);


/*
* the function response to "solve" command, move to solve state and upload board
*/
void solveCommand(char* path, Game* game);

/*
* upload board from path
*/
Board* getUserBoard(Game* game, char* path);

void editCommand(char* path, Game* game);

/*
* the function response to "mark_errors" command, the x parameter is 1 for display mark errors in the board and 0 otherwise.
*/
void markErrorsCommand(char* input, Game* game, int maxNum);

/*
* the function response to "print_board" command and prints the board.
*/
void printCommand(Game* game);

/**
 * set the value val in cell <row,col> according to user's command.
 * if the user tries to set a fixed cell, the function prints - "Error: cell is fixed"
 * if the user value is invalid (the value is already in the same box, row or column),
 * the function prints -"Error: value is invalid"
 * if the user set the last empty cell correctly the function prints- "Puzzle solved successfully"
 */
int setCommand(Game* game, int row, int col, int val);

/**
 * validates that thr current state of the board is solvable.
 * solved the generated board with deterministic backtracking.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
int validateCommand(Game* game);


/*
* the function guesses a solution to the current board using LP with thershold.
* if the board is erroneous the function prints error and the command is not executed.
*/
int guessCommand(Game* game, float threshold);

/*
* the function response to "generate" command. 
* the function generate a board by randomly choose x cells to fill and solved to board and then fill only y cells and empty all other cells
* if one of the X randomly-chosen cells has no legal value available or the resulting board has no solution the function reset the board back to the original state
* and repeat previous step. After 1000 such iteratons, treat this as an error in the puzzle genetartor.
*/
void generateCammand(Game* game, int x, int y);

/* whenever the user makes a move (via "set,", "autofill", "generate", or "guess"), the redo
* part of the list is cleared, i.e., all items beyond the current pointer are removed, the new
* move is added to the end of the list and marked as the current move, i.e., the pointer is
* updated to point to it.
*/
void addMove(Game* game, linkedList* move);

/*undo a previous move done by the user. This command is only availble
in solve and edit modes */
int undoCommand(Game* game,bool print);

/*redo a move previously undone by the user . This command is only availble
in solve and edit modes*/
int redoCommand(Game * game);

/*
* the function response to "save" command and save the board to a path.
* errorneous board or board without a solution may not be saved.
*/
void saveGame(Game* game, char* path);

/*
 * gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the solved board, and it is given even if it is incorrect for the current state of the board.
 */
void hintCommand(Game* game, char* x, char* y);

/*
 * gives a guess hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the scores Matrix calculate with LP Solver
 */
void guessHintCommand(Game* game, char* x, char* y);

int numSolution(Game* game);

void autofillCommand(Game* game);



/*
* the function response to "reset" command in 'edit' or 'solve' mode.
* the function goes over the entire undo/redo list and revert all moves.
*/
void resetCommand(Game* game);

/**
 * the function prints "Exiting..." , frees all memory resources and exits.
 */
void exiting(Game* game);

void freeBoard(Board* currentBoard);

/**
 * check the user value during setCommand.
 * check if the value is invalid (if the value is already in the same box, row or column).
 * if the value is valid returns true, else returns false.
 */
/**bool isValidSet(Board* userBoard,index ind, int val);
**/

#endif
