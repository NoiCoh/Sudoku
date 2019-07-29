#ifndef SOLVER_H
#define SOLVER_H
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

typedef enum {solved,unsolved} SudokuSolved;

typedef struct {
    int n;
    int m;
}blocksize;

enum modes {edit, solve, init};

typedef struct cell{
    int value;
    bool fixed;
    bool error;
    bool userInput;
    int *options;
    int optionsSize;
} Cell;

typedef struct Board{
    bool solved;
    Cell **cells;
    bool erroneous;
    blocksize blocksize;
} Board;

typedef struct {
	bool solvable;
	Board* solBoard;
}ILPsol;

typedef struct {
    int col;
    int row;
}index;

typedef struct Game{
	doublyLinkedList *userMoves;
	doublyNode* curMove;
	enum modes mode;
	bool markErrors;
    Board* board;
    Board* solBoard;
    double ***scores;
	FILE * ptr;
}Game;

/**
 * the random recursive algorithm to solve a sudoku board.
 * the function checks if the board is solvable. if the board is solvable return solved,
 * else return unsolved.
 */
SudokuSolved RandBacktracking(Game* game);
/**
 * an auxiliary function for the random backtracking algorithm.
 * if there is no valid options for a specific cell, the algorithm leaves the cell blank
 * and moves back (backtracks) to the previous cell.
 */
SudokuSolved goBackRand(Board* board, index ind);
/**
 * the deterministic recursive algorithm to solve a Sudoku board.
 * the function checks if the board is solvable. if the board is solvable return solved,
 * else return unsolved.
 */
SudokuSolved deterministicBacktracking(Board* board);

/**
 * the function chooses a random value from the options array of a specific cell.
 * when only a single legal value remains, the function returns that value.
 * if there are no options remains -returns 0.
 */
int getRandValue(Board* board,index ind);

/**
 * the function creates an array of valid options for a specific cell.
 */
void findOptionsCell(Game* game,index ind);

/**
 * the function checks if @param value is in the same row ,column and box of the cell in @param index.
 * if so returns false. else, returns true.
 * if mark=true we mark erroneous cells, if mark=false we unmark erroneous cells
 * (for example,when setting a cell to zero).
 */
bool isValidOption(Game* game,index ind,int value,bool mark);

/**
 * the function finds the starting cell index of the the block that @param index belongs to.
 */
index findBoxIndex(Game* game, index index);

/**
 * the function checks if the @param value is in the block starting at @param index block. if mark=true ,
 * the function marks errornous cells in the same block.
 */
bool checkInBox(Game* game,index box,int value,bool mark);

/**
 * the function check if @param value is in the same row and column.
 *  if so returns false. else, returns true. if mark=true ,
 *  the function marks errornous cells in the same row and column.
 */
bool checkInRowAndCol(Game* game,index index, int value,bool mark);

/**
 * the function checks the size of the options array for a specific cell.
 */
void checkSizeArray(Board* board,index ind);

/**
 * the function removes zero elements from option array.
 */
void removeZeroFromOptions(Board* board, index ind);

/**
 * the function checks if there is an empty cell in the board.
 * if so, returns true. else, returns false.
 */
bool IsThereEmptyCell(Board* board);

/**
 * the function returns the index of the first empty cell.
 */
index FindEmptyCell(Board* board);

#endif
