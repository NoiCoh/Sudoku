#ifndef GAME_H
#define GAME_H
#include "Solver.h"
#include "Parser.h"
#include "ILPsolver.h"
#include "FileAux.h"
#include "GameAux.h"

 /*
 * the function response to "solve" command, enters solve mode and upload the board from
 * the user's path.
 */
void solveCommand(char* path, Game* game);

/*
* the function response to "edit" command, enters edit mode and upload the board from
* the user's path. if no path is given, the function enters edit mode with an empty 9X9 board.
*/
void editCommand(char* path, Game* game);

/*
* the function response to "mark_errors" command, the x parameter is 1 for display mark errors in the board and 0 otherwise.
*/
void markErrorsCommand(char* input, Game* game);

/*
* the function response to "print_board" command and prints the board.
*/
void printCommand(Game* game);

/**
 * the function response to "set" command.
 * set the value val in cell <col,row> according to user's command.
 * in solve mode -if the user sets the last empty cell correctly, the function prints- "Puzzle solved successfully"
 * and enters init mode. if it is an errornous value, the function prints-"Error: The board is erroneous!".
 * the user may set a value into a fixed cell only in edit mode.
 */
void setCommand(Game* game, int row, int col, int val,bool addToMoveList);

/**
 * the function response to "validate" command.
 * validates that the current state of the board is solvable.
 * solve the generated board with the Linear programming algorithm.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 * return value: 1 - if validation passed, 0 - otherwise.
 */
int validateCommand(Game* game, int isSave);

/*
* the function response to "guess" command.
* the function guesses a solution to the current board using LP with a thershold.
* if the board is erroneous, the function prints an error and the command is not executed.
* return value: 1 - guess command succeeded , 0 - otherwise.
*/
int guessCommand(Game* game, float threshold);

/*
* the function response to "generate" command.
* the function generate a board by randomly filling x empty cells with legal values, running ILP to solve the board, and then clearing all but y random cells .
* if one of the X randomly-chosen cells has no legal value available or the resulting board has no solution- the function reset the board back to the original state
* and repeat previous step. After 1000 such iteratons, treat this as an error in the Soduko genetartor.
*/
void generateCommand(Game* game, int x, int y);

/*
* the function response to "undo" command.
* undo a previous move done by the user.
*/
int undoCommand(Game* game, bool print);

/*
* the function response to "redo" command.
* redo a move previously undone by the user.
*/
int redoCommand(Game * game);

/*
* the function response to "save" command and saves the board to a given path.
* errorneous board or board without a solution may not be saved.
*/
void saveGame(Game* game, char* path);

/*
 * the function response to "hint" command and gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the solved board even if it is incorrect for the current state of the board.
 */
void hintCommand(Game* game, char* x, char* y);

/*
 * the function response to "guess_hint" command.
 * the function gives a guess hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the scores Matrix calculated with the LP Solver algorithm.
 */
void guessHintCommand(Game* game, char* x, char* y);

/*
 * the function response to "num_solutions" command and prints the number of solutions for the current board.
 * the function uses the exhaustive backtracking algorithm to count all options to solve the board.
 * if the board is errornous it is an error.
 */
int numSolution(Game* game);

/*
 * the function response to "autofill" command.
 * the function fill cells which contains a single legal value.
 * if the board is errornous it is an error.
 */
void autofillCommand(Game* game);

/*
* the function response to "reset" command in 'edit' or 'solve' mode.
* the function goes over the entire undo/redo list and revert all moves.
*/
void resetCommand(Game* game);

/*
* the function response to "exit" command.
* the function prints "Exiting..." , frees all memory resources and terminates the program .
*/
void exiting(Game* game);

#endif
