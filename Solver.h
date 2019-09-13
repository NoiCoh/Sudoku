#ifndef SOLVER_H
#define SOLVER_H
#include "linkedList.h"
#include "MainAux.h"
#include "Stack.h"

/**
 * the deterministic algorithm to solve a Sudoku board with a stack implementation.
 * return value: SudokuSolved struct that indicates if the sudoku board is solved.
 */
SudokuSolved deterministicBacktrackingWithStack(Game* game, int N);

/*
 * count the number of solutions of the current board using backtracking algorithm and returns it.
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
