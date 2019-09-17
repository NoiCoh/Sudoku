/*
 * ParserAux:
 * This module contains auxiliary functions for Parser module.
 */
#include "ParserAux.h"

/*
* counts how many arguments the user entered while writing a command.
* return value: returns the count.
*/
int argsNum(char* move[]) {
	int cnt, i;
	cnt = 0;
	i = 0;
	while (move[i] != NULL) {
		cnt++;
		i++;
	}
	return cnt;
}
/*
* checks if the number of parameters that the user entered fits the syntax of the command.
* if no, prints an error and notify the user the correct syntax of the command.
* return value: 0 - if there is an error, 1- otherwise.
*/
int checkParamsNum(int validNum, int paramsNum, Command c, int maxVal) {
	if (paramsNum == validNum) {
		return 1;
	}
	if (paramsNum < validNum) {
		printf("Error: Not enough parameters were entered.\n");
	}
	else if (paramsNum > validNum) {
		printf("Error: Extra parameters were entered.\n");
	}

	printCommandSyntax(c, maxVal);
	return 0;

}

/*
* checks if a the number's type is a float.
* return value: 1 - if the number in type float, 0 - otherwise.
*/
int isFloat(char* num) {
	int i;
	i = 0;
	while (num[i] != '\0') {
		if (!(isNum(num[i]) || (num[i] == '.'))) {
			return 0;
		}
		i++;
	}
	if (num[0] == '0' && num[1] == '.') {
		return 1;
	}
	return 0;
}

/*
* checks if the char is a number.
* return value: 1 if a digit is an integer in the range [0-9], 0 - otherwise.
*/
int isNum(char c) {
	if (c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

/*
* checks if a string @move is a number
* return value: 1 - if the string if a number, 0 - otherwise.
*/
int isNums(char* move) {
	int i;
	i = 0;
	while (move[i] != '\0') {
		if (isNum(move[i]) == 0) {
			return 0;
		}
		i++;
	}
	return 1;
}

/*
* checks if the @value param is in the range [@min,@max]
* return value: 1- if the value is in range. 0 - otherwise.
*/
int isInRange(int value, int max, int min) {
	if (value <= max && value >= min) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF() {
	if (feof(stdin)) {
		printf("Exiting...\n");
		exit(0);
	}
}

/*-----validate the command's syntax, number of parameters, parameter's type and range-----*/

/*
* validate solve command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateSolve(char* move[]) {
	int paramsNum, checkparamsnum;
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, solve, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

/*
* validate edit command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateEdit(char* move[]) {
	int paramsNum;
	paramsNum = argsNum(move);
	if (paramsNum > 2) {
		printf("Error: Extra parameters were entered.\n");
		printCommandSyntax(edit, 0);
		return 0;
	}
	return 1;
}

/*
* validate mark errors command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateMarkErrors(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, mark_errors, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

/*
* validate print board command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validatePrintBoard(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(1, paramsNum, print_board, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

/*
* validate set command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateSet(char* move[], Game* game) {
	int paramsNum, checkparamsnum, n, m, maxValue;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	n = game->board->blocksize.n;
	m = game->board->blocksize.m;
	maxValue = n * m;

	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(4, paramsNum, set, maxValue);
	if (!checkparamsnum) {
		return 0;
	}

	if (!(isValidSetParams(move[1], move[2], move[3], game))) {
		return 0;
	}
	return 1;
}

/*
* validate solve command's parameters.
* return value: 1 - validation passed, 0 - otherwise.
*/
int isValidSetParams(char* x, char* y, char* z, Game* game) {
	int maxValue, m, n;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	maxValue = n * m;
	if (!(isNums(x) && isInRange(atoi(x), maxValue, 1))) {
		printlegalRange("first", "integer", 1, maxValue);
		return 0;
	}
	if (!(isNums(y) && isInRange(atoi(y), maxValue, 1))) {
		printlegalRange("second", "integer", 1, maxValue);
		return 0;
	}
	if (!(isNums(z) && isInRange(atoi(z), maxValue, 0))) {
		printlegalRange("third", "integer", 0, maxValue);
		return 0;
	}
	return 1;
}

/*
* validate validate command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateValidate(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	if (game->board == NULL) {
		printBoardNotInit();
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(1, paramsNum, validate, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

/*
* validate guess command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateGuess(char* move[], Game* game) {
	int paramsNum, checkparamsnum, x;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, guess, 0);
	if (!checkparamsnum) {
		return 0;
	}
	if (!isFloat(move[1])) {
		printlegalRange("first", "integer", 0, 1);
		return 0;
	}
	else {
		x = atof(move[1]);
		if (!(isInRange(x, 1, 0))) {
			printlegalRange("first", "integer", 0, 1);
			return 0;
		}
	}
	return 1;
}

/*
* validate generate command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateGenerate(char* move[], Game* game) {
	int paramsNum, checkparamsnum, n, m, N;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = m * n;
	if (game->mode != editMode) {
		if (game->mode == solveMode) {
			printErrorMode("solve");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(3, paramsNum, generate, N * N);
	if (!checkparamsnum) {
		return 0;
	}
	if (!(isValidTwoParams(move[1], move[2], 0, N * N))) {
		return 0;
	}

	return 1;
}

/*
* validate the parameters for a command that receives 2 parameters. 
* return value: 1 - validation passed, 0 - otherwise.
*/
int isValidTwoParams(char* x, char* y, int minValue, int maxValue) {
	if (!isNums(x) || !isInRange(atoi(x), maxValue, minValue))
	{
		printlegalRange("first", "integer", minValue, maxValue);
		return 0;
	}
	if (!isNums(y) || !isInRange(atoi(y), maxValue, minValue))
	{
		printlegalRange("second", "integer", minValue, maxValue);
		return 0;
	}
	return 1;
}

/*
* validate undo and redo command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateUndoAndRedo(char* move[], Game* game, int isUndo) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	if (isUndo) {
		checkparamsnum = checkParamsNum(1, paramsNum, undo, 0);
	}
	else {
		checkparamsnum = checkParamsNum(1, paramsNum, redo, 0);
	}
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

/*
* validate save command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateSave(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, save, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}
/*
* validate hint and guess hint commands syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateHintAndGuessHint(char* move[], Game* game, int isHint) {
	int paramsNum, checkparamsnum, n, m, maxValue;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	maxValue = m * n;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	if (isHint) {
		checkparamsnum = checkParamsNum(3, paramsNum, hint, maxValue);
	}
	else {
		checkparamsnum = checkParamsNum(3, paramsNum, guess_hint, maxValue);
	}
	if (!checkparamsnum) {
		return 0;
	}
	if (!isValidTwoParams(move[1], move[2], 1, maxValue)) {
		return 0;
	}
	return 1;
}
/*
* validate autofill command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateAutofill(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(1, paramsNum, autofill, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}
/*
* validate number solution, exit and reset commands syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateNumSolAndExitAndReset(char* move[], Game* game, Command c, int isExit) {
	int paramsNum, checkparamsnum;
	if (isExit == 0) {
		if (game->mode == initMode) {
			printErrorMode("init");
			return 0;
		}
	}
	paramsNum = argsNum(move);
	if (c == num_solutions) {
		checkparamsnum = checkParamsNum(1, paramsNum, num_solutions, 0);
	}
	else if (c == exitCommand) {
		checkparamsnum = checkParamsNum(1, paramsNum, exitCommand, 0);
	}
	else if (c == reset) {
		checkparamsnum = checkParamsNum(1, paramsNum, reset, 0);
	}
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}
/*---------------------------------------------------------------------------------------*/

/*prints the correct syntaxt of a command*/
void printCommandSyntax(Command c, int maxVal) {
	if (c == solve) {
		printf("The correct syntax for solve command is: solve x.\nx should include a full or relative path to the file.\n");
	}
	if (c == edit) {
		printf("The correct syntax for edit command is: edit x.\nx should include a full or relative path to the file \
		and this parameter is optional.\nIf no parameter is supplied, the default board is an empty 9X9 board\n");
	}
	if (c == mark_errors) {
		printf("The correct syntax for mark_errors command is: mark_errors x.\nX should be either 0 or 1.\n");
	}
	if (c == print_board) {
		printf("The correct syntax for print_board command: print_board (with no extra parameters).\n");
	}
	if (c == set) {
		printf("The correct syntax for set command: set x y z (sets the value of cell<x,y> to z).\n");
		printf("x and y are integers between 1 to %d.\n", maxVal);
		printf("z is an integer between 0 to %d.\n", maxVal);
	}
	if (c == validate) {
		printf("The correct syntax for validate command:validate (with no extra parameters).\n");
	}
	if (c == guess) {
		printf("The correct syntax for guess command:guess x.\nX should be a float in range [0-1].\n");
	}
	if (c == generate) {
		printf("The correct syntax for generate command:generate x y.\n");
		printf("X and y are integers between 0 to %d.\n", maxVal);
	}
	if (c == undo) {
		printf("The correct syntax for undo command:undo (with no extra parameters).\n");
	}
	if (c == redo) {
		printf("The correct syntax for redo command:redo (with no extra parameters).\n");
	}
	if (c == save) {
		printf("The correct syntax for save command is: save x.\nX includes a full or relative path to the file.\n");
	}
	if (c == hint) {
		printf("The correct syntax for hint command is: hint x y\n");
		printf("X and y are integers between 1 to %d.\n", maxVal);
	}
	if (c == guess_hint) {
		printf("The correct syntax for guess_hint command is: guess_hint x y\n");
		printf("X and y are integers between 1 to %d.\n", maxVal);
	}
	if (c == num_solutions) {
		printf("The correct syntax for num_solutions command:num_solutions (with no extra parameters).\n");
	}
	if (c == autofill) {
		printf("The correct syntax for autofill command:autofill (with no extra parameters).\n");
	}
	if (c == reset) {
		printf("The correct syntax for reset command:reset (with no extra parameters).\n");
	}
	if (c == exitCommand) {
		printf("The correct syntax for exit command:exit (with no extra parameters).\n");
	}

}

