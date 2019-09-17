/*
 * MainAux:
 * This module contains generality auxiliary functions and error printing functions.
 * Also, This module implements structs and enums for the game.
 */
#ifndef MAINAUX_H
#define MAINAUX_H
#include <stdbool.h>
#include "Stack.h"
#include "linkedList.h"

/* the enum represents a sudoku solved status*/
typedef enum { solved, unsolved } SudokuSolved;

/* the enum represents the game current mode*/
typedef enum { editMode, solveMode, initMode } modes;

/* the struct represents a data of a cell - value and if the cell is fixed*/
typedef struct {
	int num;
	int fixed;
}data;

/* the struct represents a block size */
typedef struct {
	int n;
	int m;
}blocksize;

/* the struct represents a cell in the board
* the struct contains : value, if it is fixed, if the cell is errorneous, if the user sets this value,
* the list of valid value options for thie cell and it's size, and the mapping list for solution.
*/
typedef struct cell {
	int value;
	bool fixed;
	bool error;
	bool userInput;
	int* options;
	int optionsSize;
	int* ixMap;
} Cell;

/* the struct represents the game's board
* the struct contains : the cells, it's size and if it is erroneous or not.
*/
typedef struct Board {
	Cell** cells;
	bool erroneous;
	blocksize blocksize;
} Board;

/* the struct represents the solution from LP algorithm */
typedef struct {
	int solvable;
	double* solBoard;
}LPsol;

/* the struct represents the game.
* the struct contains : the board, a list of the user moves and a pointer to the current move, the game current mode,
* and if the game should show the user the errors
*/
typedef struct Game {
	doublyLinkedList* userMoves;
	doublyNode* curMove;
	modes mode;
	bool markErrors;
	Board* board;
}Game;

/* the enum represents all the valid commands in the game */
typedef enum {
	solve,
	edit,
	mark_errors,
	print_board,
	set,
	validate,
	guess,
	generate,
	undo,
	redo,
	save,
	hint,
	guess_hint,
	num_solutions,
	autofill,
	reset,
	exitCommand
}Command;

/**
 * restart the game by starting over with the initialization procedure.
 */
void restart();

/*------------------------Error printing------------------------*/

/* prints error and informs the user which parameter is not valid and it's legal range and type */
void printlegalRange(char* type, char* param, int minNum, int maxNum);

/* prints error if the board is not initilized */
void printBoardNotInit();

/* prints error if the command is unavailable in a specific mode*/
void printErrorMode(char* mode);

/* prints error if the board is erroneous*/
void printErroneousBoardError();

/**
 * If a standard failure or memory error occurs (failure of malloc, scanf, etc.)
 * the program prints "Error: <function-name> has failed" and exits.
 */
void funcFailed(char* str);

/*--------------------------------------------------------------*/

/**
 * initialize board values and params to zero and returns the initialized board.
 */
Board* initialize(blocksize block);

/* initialize struct game and allocate memory and returns the initialize game */
Game* initializeGame();


/*updates game's mode or board*/
void UpdateGame(Game* game, Board *userBoard, modes mode);

/**
 * Prints the current state of @param board. if the markErrors parameter is true-
 cells that participate in an error should be marked .
 */
void printBoard(Game* game);

/*an auxilary function for printing the board*/
void printNTimes(int n);

/**
 * Make a deep copy of @param board to @param copyBorad.
 */
void makeCopyBoard(Board* borad, Board* copyBorad);

/**
 *check if board has a cell that marked as erroneous.
 */
bool isBoardErroneous(Board* board);

/*marks cells which are errornous*/
void markErroneousCells(Game* game);

/**
 * the function checks if there is an empty cell in the board.
 * if so, returns true. else, returns false.
 */
bool IsThereEmptyCell(Board* board, int N);

/**
 * the function checks if @param value is in the same row ,column and box of the cell in @param index.
 * if so returns false. else, returns true.
 * if mark=true we mark erroneous cells, if mark=false we unmark erroneous cells
 * (for example,when setting a cell to zero).
 */
bool isValidOption(Game* game, index ind, int value, bool mark, bool checkOnlyFixed);

/*
* the function checks if the board contains at least 2 fixed cells with the same value in the same row, colunm or box.
* return value: 1 - if there is an errornous in a fixed cell, 0 - otherwise.
*/
int isFixedErrornous(Game* game);

/**
 * the function finds the starting cell index of the the block that @param index belongs to.
 */
index findBoxIndex(Game* game, index index);

/**
 * the function checks if the @param value is in the block starting at @param index block. if mark=true ,
 * the function marks errornous cells in the same block.
 */
bool checkInBox(Game* game, index box, index ind, int value, bool mark, bool checkOnlyFixed);

/**
 * the function check if @param value is in the same row and column.
 *  if so returns false. else, returns true. if mark=true ,
 *  the function marks errornous cells in the same row and column.
 */
bool checkInRowAndCol(Game* game, index index, int value, bool mark, bool checkOnlyFixed);

/*
* calculate size of a row/col in the sudoku game board from block size and returns it.
*/
int calculateNfromGame(Game* game);

/**
 * an auxilary function that free the board memory
 */
void freeBoard(Board* currentBoard);

/**
 * an auxilary function that free the game memory
 */
void freeGame(Game* game);

/**
 * the function creates an array of valid options for a specific cell.
 */
void findOptionsCell(Game* game, index ind);

/**
 * the function removes zero elements from option array.
 */
void removeZeroFromOptions(Game* game, index ind);




#endif
