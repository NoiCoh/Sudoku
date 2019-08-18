#ifndef SOLVER_H
#define SOLVER_H
#include "linkedList.h"
#include "MainAux.h"
#include "Stack.h"


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
SudokuSolved goBackRand(Game* game, index ind);
/**
 * the deterministic recursive algorithm to solve a Sudoku board.
 * the function checks if the board is solvable. if the board is solvable return solved,
 * else return unsolved.
 */
SudokuSolved deterministicBacktracking(Game* game, int N);

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
 * the function checks the size of the options array for a specific cell.
 */
void checkSizeArray(Board* board,index ind);

/**
 * the function removes zero elements from option array.
 */
void removeZeroFromOptions(Board* board, index ind);


/**
 * the function returns the index of the first empty cell.
 */
index FindEmptyCell(Board* board,int N);

/**
 * the deterministic algorithm to solve a Sudoku board with a stack implementation
 */
SudokuSolved deterministicBacktrackingWithStack(Game* game, int N);

 /**
  * count the number of solutions of the current board using backtracking algorithm.
  */
int exhaustiveBacktracking(Game* game, int N);

/**
 * an auxiliary function that finds the previous cell index
 */
void pre(int* i, int* j, int N);

#endif
