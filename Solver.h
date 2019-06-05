#ifndef SOLVER_H
#define SOLVER_H
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {solved,unsolved} SudokuSolved;

typedef struct cell{
    int value;
    bool fixed;
    bool userInput;
    int *options;
    int optionsSize;
} Cell;

typedef struct Board{
    bool solved;
    Cell **cells;
} Board;

typedef struct {
    int col;
    int row;
}index;

/**
 * the random recursive algorithm to solve a sudoku board.
 * check if the board is solvable and return solved, else return unsolved.
 */
SudokuSolved RandBacktracking(Board* board);
/**
 * Auxiliary function for random backtracking algorithm.
 * if there is no valid options for a specific cell, then the algorithm leaves the cell blank
 * and moves back (backtracks) to the previous cell.
 */
SudokuSolved goBackRand(Board* board, index ind);
/**
 * the deterministic recursive algorithm to solve a Sudoku board.
 * check if the board is solvable and return solved, else return unsolved.
 */
SudokuSolved deterministicBacktracking(Board* board);

/**
 * choose a random value from option array of a specific cell.
 * when only a single legal value remains, the function returns that value.
 * if there are no options remains return 0.
 */
int getRandValue(Board* board,index ind);

/**
 * create an array of valid options for a specific cell.
 */
void findOptionsCell(Board* board,index ind);

/**
 * check if @param value is in the same row ,column and box of the cell in @param index.
 * if so return false. else, return true.
 */
bool isValidOption(Board* board,index ind,int value);

/**
 * find the starting cell index of the the box that @param index is belong to.
 */
index findBoxIndex(index index);

/**
 * check if the @param value is in the box starting at @param index box.
 */
bool checkInBox(Board* board,index box,int value);

/**
 * check if @param value is in the same row and column.
 *  if so return false. else, return true.
 */
bool checkInRowAndCol(Board* board,index index, int value);

/**
 * check size of option array for a specific cell.
 */
void checkSizeArray(Board* board,index ind);

/**
 * remove zero elements from option array.
 */
void removeZeroFromOptions(Board* board, index ind);

/**
 * check if there is an empty cell in the board.
 * if so, return true. else, return false.
 */
bool IsThereEmptyCell(Board* board);

/**
 * return the index of the first empty cell.
 */
index FindEmptyCell(Board* board);

#endif
