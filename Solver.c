/*
 * Solver:
 * This module implements backtracking algorithm to count the number of solution for board's game.
 */
#include "Solver.h"

/**
 * The function implements a deterministic backtracking algorithm to solve a Sudoku board with a stack.
 * return value: SudokuSolved struct that indicates if the sudoku board is solved.
 */
SudokuSolved deterministicBacktrackingWithStack(Game* game, int N) {
	int val;
	index ix;
	bool tried_all;
	Stack stack;
	cellMem* prevCellMem;
	val = 1;
	initStack(&stack);
	tried_all = false;
	while(IsThereEmptyCell(game->board, N)) {
		if (!tried_all) val = 1;
		ix = FindEmptyCell(game->board, N);
		tried_all = true;
		for (; val <= N; val++) {
			if (isValidOption(game, ix, val, false,false)) {
				game->board->cells[ix.row][ix.col].value = val;
				push(ix,val,&stack);
				tried_all = false;
				break;
			}
		}
		if (tried_all) {
			if (stackIsEmpty(&stack)) {
				return unsolved;
			}
			prevCellMem = pop(&stack); /* previous index we handle in the stack */
			game->board->cells[prevCellMem->ix.row][prevCellMem->ix.col].value = 0; 
			val = prevCellMem->val;
			val = val + 1;
		}
	}
	return solved;
}

/*
 * the function count the number of solutions of the current board using backtracking algorithm.
 * return value: returns the count.
 */
int exhaustiveBacktracking(Game* game, int N) {
	int counter, k, i, j, val;
	index ind;
	bool doneGoBack;
	counter = 0;
	while (1) {
		if (deterministicBacktrackingWithStack(game, N) == solved) {
			counter++;
			i = N - 1;
			j = N - 1;
		}
		else {
			ind = FindEmptyCell(game->board, N);
			j = ind.col;
			i = ind.row;
			pre(&i, &j, N);
			while (game->board->cells[i][j].fixed == true || game->board->cells[i][j].userInput == true) {
				if (i == 0 && j == 0) {
					return counter;
				}
				pre(&i, &j, N);
			}
		}
		doneGoBack = false;
		while (doneGoBack == false) {
			val = game->board->cells[i][j].value;
			for (k = val + 1; k <= N; k++) {
				ind.col = j;
				ind.row = i;
				if (isValidOption(game, ind, k, false,false)) {
					game->board->cells[i][j].value = k;
					doneGoBack = true;
					break;
				}
			}
			if (doneGoBack == false) {
				game->board->cells[i][j].value = 0;
				if (i == 0 && j == 0) {
					return counter;
				}
				pre(&i, &j, N);
				while (game->board->cells[i][j].fixed == true || game->board->cells[i][j].userInput == true) {
					if (i == 0 && j == 0) {
						return counter;
					}
					pre(&i, &j, N);
				}
			}
		}
	}
}

/*--------------------Auxiliary Functions for Solver--------------------*/
/**
* an auxiliary function that finds the previous cell index
*/
void pre(int* i, int* j,int N) {
	if (*j == 0) {
		*i = *i - 1;
		*j = N-1;
	}
	else {
		*j = *j - 1;
	}
}

/**
 * the function returns the index of the first empty cell.
 */
index FindEmptyCell(Board* board, int N) {
	int row, col;
	index i;
	i.col = 0;
	i.row = 0;
	for (row = 0; row < N; row++) {
		for (col = 0; col < N; col++) {
			if (board->cells[row][col].value == 0) {
				i.col = col;
				i.row = row;
				return i;
			}
		}
	}
	return i;
}