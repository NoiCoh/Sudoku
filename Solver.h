/*
 * Solver:
 * This module implements backtracking algorithm to count the number of solution for board's game.
 */

#ifndef SOLVER_H
#define SOLVER_H
#include "linkedList.h"
#include "MainAux.h"
#include "Stack.h"

/**
 * The function implements a deterministic backtracking algorithm to solve a Sudoku board with a stack.
 * return value: SudokuSolved struct that indicates if the sudoku board is solved.
 */
SudokuSolved deterministicBacktrackingWithStack(Game* game, int N);

/*
 * the function count the number of solutions of the current board using backtracking algorithm.
 * return value: returns the count.
 */
int exhaustiveBacktracking(Game* game, int N);

/*--------------------Auxiliary Functions for Solver--------------------*/
/**
 * an auxiliary function that finds the previous cell index
 */
void pre(int* i, int* j, int N);

/**
 * the function returns the index of the first empty cell.
 */
index FindEmptyCell(Board* board, int N);

#endif
