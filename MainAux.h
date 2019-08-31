#ifndef MAINAUX_H
#define MAINAUX_H
#include <stdbool.h>
#include "Stack.h"
#include "linkedList.h"

typedef enum { solved, unsolved } SudokuSolved;

typedef struct {
	int num;
	int fixed;
}data;

typedef struct {
	int n;
	int m;
}blocksize;

typedef enum { editMode, solveMode, initMode } modes;

typedef struct cell {
	int value;
	bool fixed;
	bool error;
	bool userInput;
	int* options;
	int optionsSize;
	int* ixMap;
} Cell;

typedef struct Board {
	Cell** cells;
	bool erroneous;
	blocksize blocksize;
} Board;

typedef struct {
	int solvable;
	double* solBoard;
}LPsol;

typedef struct Game {
	doublyLinkedList* userMoves;
	doublyNode* curMove;
	modes mode;
	bool markErrors;
	Board* board;
}Game;

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

/*------------------------Error printing------------------------*/
void printlegalRange(char* type, char* param, int minNum, int maxNum);

void printBoardNotInit();

void printErrorMode(char* mode);

void printErroneousBoardError();

/**
 * If a standard failure or memory error occurs (failure of malloc, scanf, etc.)
 * the program prints "Error: <function-name> has failed" and exits.
 */
void funcFailed(char* str);

/*--------------------------------------------------------------*/

/*an auxilary function for printing the board*/
void printNTimes(int n);

/*if the user' command is "edit" with no edditional parameter (path), the user gets an empty 9X9 board*/
Board* createDefaultBoard();

/*if the board is solved, notify the user and enter init mode. else- notify that the board is errornous*/
int checkIfBoardSolved(Game* game);

/**
 * initialize board values and params to zero.
 */
Board* initialize(blocksize block);

/*initialize struct game and allocate memory */
Game* initializeGame();


/*updates game's mode or board*/
void UpdateGame(Game* game, Board *userBoard, modes mode);

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
 *check if board has a cell that marked as erroneous.
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

int getLegalGuess(Game* game, LPsol* lpSol, int row, int col, float threshold, int* legalValues, double* scores);

/*weighted random selection from the scores array */
int getRandIndex(int numOflegalValues, double* scores);

/*creates a linked list with all of the board's changes after calling "generate" command.*/
linkedList* createGenerateMoveList(Board* newBoard, Board* orignalBoard);

/*prints the correct syntaxt of a command*/
void printCommandSyntax(Command c, int maxVal);

/*counts how many arguments the user entered while writing a command*/
int argsNum(char* move[]);

/*checks if the number of parameters that the user entered fits the syntax of the command.
*if no, prints an error and notify the user the correct syntax of the command*/
int checkParamsNum(int validNum, int paramsNum, Command c, int maxVal);

/*checks if a the number's type is a float*/
int isFloat(char* num);

/*checks if a digit is an integer in the range [0-9]*/
int isNum(char move);

/*checks if a string @move is a number*/
int isNums(char* move);

/*checks if the @value param is in the range [@min,@max]*/
int isInRange(int value, int max, int min);

/*-----validate the command's syntax, number of parameters, parameter's type and range-----*/
int validateSolve(char* move[]);

int validateEdit(char* move[]);

int validateMarkErrors(char* move[], Game* game);

int validatePrintBoard(char* move[], Game* game);

int validateSet(char* move[], Game* game);

int isValidSetParams(char* x, char* y, char* z, Game* game);

int validateValidate(char* move[], Game* game);

int validateGuess(char* move[], Game* game);

int validateGenerate(char* move[], Game* game);

int isValidTwoParams(char* x, char* y, int minValue, int maxValue);

int validateUndoAndRedo(char* move[], Game* game, int isUndo);

int validateSave(char* move[], Game* game);

int validateHintAndGuessHint(char* move[], Game* game, int isHint);

int validateAutofill(char* move[], Game* game);

int validateNumSolAndExitAndReset(char* move[], Game* game, Command c);
/*---------------------------------------------------------------------------------------*/

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
bool isValidOption(Game* game, index ind, int value, bool mark);

/**
 * the function finds the starting cell index of the the block that @param index belongs to.
 */
index findBoxIndex(Game* game, index index);

/**
 * the function checks if the @param value is in the block starting at @param index block. if mark=true ,
 * the function marks errornous cells in the same block.
 */
bool checkInBox(Game* game, index box, index ind, int value, bool mark);

/**
 * the function check if @param value is in the same row and column.
 *  if so returns false. else, returns true. if mark=true ,
 *  the function marks errornous cells in the same row and column.
 */
bool checkInRowAndCol(Game* game, index index, int value, bool mark);


#endif
