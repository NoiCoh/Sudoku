#ifndef MAINAUX_H
#define MAINAUX_H
#include "Solver.h"

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

void printCommandSyntax(Command c, int maxVal);

void printlegalRange(char* type, char* param, int minNum, int maxNum);
int isValidTwoParams(char* x, char* y, int minValue, int maxValue);

int validateSolve(char* move);
int validateEdit(char* move);
int validateMarkErrors(char* move, Game* game);
int validatePrintBoard(char* move, Game* game);
int validateSet(char* move, Game* game);
int validateValidate(char* move, Game* game);
int validateGuess(char* move, Game* game);
int validateGenerate(char* move, Game* game, int N);
int validateSave(char* move, Game* game);
int validateUndoAndRedo(char* move, Game* game, int isUndo);
int validateHintAndGuessHint(char* move, Game* game, int maxValue, int isHint);
int validateAutofill(char* move, Game* game);




void printExtraParams();
void printErrorMode(char* mode);
void printErroneousBoardError();
Board* createDefaultBoard();

void checkIfBoardSolved(Game* game);



/**
 * initialize board values and params to zero.
 */
Board* initialize(blocksize block);

Game* initializeGame();

void UpdateGame(Game* game, Board *userBoard, enum modes mode);
void printWelcome();
void printNTimes(int n);

/**
 * Prints the current state of @param board. if the markErrors parameter is true-
 cells that participate in an error should be marked .
 */
void printBoard(Game* game);

/**
 * Make a deep copy of @param board to @param copyBorad.
 */
void makeCopyBoard(Board* borad, Board* copyBorad);

/**
 * If a standard failure or memory error occurs (failure of malloc, scanf, etc.)
 * the program prints "Error: <function-name> has failed" and exits.
 */
void funcFailed(char* str);

/**
 *check if board has a cell that marked as an error.
 */
bool isBoardErroneous(Board* board);

/**
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF();


/*
* set value in board's game
*/
void setValue(Game* game, int col, int row, int value);

/*
Count how many empty cells in board's game
**/
int FindHowMuchEmptyCells(Game* game);

int getLegalGuess(Game* game, LPsol* lpSol, int row, int col, float threshold, int* legalValues);

double* getScoresOfLegalValue(LPsol* lpsol, int row, int col, int numOflegalValues, int* legalValues);

int getRandIndex(int numOflegalValues, double* scores);

linkedList* createGenerateMoveList(Board* newBoard, Board* orignalBoard);
#endif
