/*
 * GameAux:
 * This module contains auxiliary functions for Game module.
 */
#ifndef GAMEAUX_H
#define GAMEAUX_H
#include "MainAux.h"

/**
 * the function chooses a random value from the options array of a specific cell.
 * when only a single legal value remains, the function returns that value.
 * if there are no options remains returns 0.
 */
int getRandValue(Game* game, index ind);

/*
* if the user's command is "edit" with no edditional parameter (path), the user gets an empty 9X9 board.
* return value: an empty board.
*/
Board* createDefaultBoard();

/*
* if the board is solved, notify the user and enter init mode. else- notify that the board is errornous
* return value: 1 - if board is solved, 0 - otherwise.
*/
int checkIfBoardSolved(Game* game, int isSolveCommand);

/*
* set value in board's game
*/
void setValue(Game* game, int col, int row, int value);

/*
* Count how many empty cells in board's game
*/
int FindHowMuchEmptyCells(Game* game);

/*
* The function counts all legal values's scores for cell <col,row> that are bigger then the threshold
* return value : number of leagal options for cell <col,row>
*/
int getLegalGuess(Game* game, LPsol* lpSol, int row, int col, float threshold, int* legalValues, double* scores);

/*
* weighted random selection from the scores array that created by LP algorithmn .
* return value: if there is only one legal return the first option.
*				if all scores are equal to zero - choose one option randomly.
*				else, make the scores to be a relative value of the total sum and choose random number in range [0-1].
*				the function returns the first option that adding to the accumulative sum makes
*				the accumulative sum bigger than the random number.
*
*/
int getRandIndex(int numOflegalValues, double* scores);

/*
* creates a linked list with all of the board's changes after calling "generate" command.
* return value: the linked list with the changes.
*/
linkedList* createGenerateMoveList(Board* newBoard, Board* orignalBoard);

/*
* generate solved board from the Lp solve.
*/
void makeSolBoard(Game* game, LPsol* solve);

/* the function adds the user's move to a moves list that tracks user's moves in order to undo\redo them .
* Whenever the user makes a move (via "set,", "autofill", "generate", or "guess"), all items beyond the current move pointer are removed, the new
* move is added to the end of the list and marked as the current move.
*/
void addMove(Game* game, linkedList* move);
#endif
