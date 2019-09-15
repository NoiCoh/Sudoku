#ifndef PARSERAUX
#define PARSERAUX
#include "MainAux.h"

/*
* counts how many arguments the user entered while writing a command and returns the count.
*/
int argsNum(char* move[]);

/*
* checks if the number of parameters that the user entered fits the syntax of the command.
* if no, prints an error and notify the user the correct syntax of the command.
* return value: 0 - if there is an error, 1- otherwise.
*/
int checkParamsNum(int validNum, int paramsNum, Command c, int maxVal);

/*
* checks if a the number's type is a float.
* return value: 1 - if the number in type float, 0 - otherwise.
*/
int isFloat(char* num);

/*
* checks if the char is a number.
* return value: 1 if a digit is an integer in the range [0-9], 0 - otherwise.
*/
int isNum(char move);

/*
* checks if a string @move is a number
* return value: 1 - if the string if a number, 0 - otherwise.
*/
int isNums(char* move);

/*
* checks if the @value param is in the range [@min,@max]
* return value: 1- if the value is in range. 0 - otherwise.
*/
int isInRange(int value, int max, int min);

/**
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF();

/*-----validate the command's syntax, number of parameters, parameter's type and range-----*/

/*
* validate solve command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateSolve(char* move[]);

/*
* validate edit command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateEdit(char* move[]);

/*
* validate mark errors command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateMarkErrors(char* move[], Game* game);

/*
* validate print board command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validatePrintBoard(char* move[], Game* game);

/*
* validate set command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateSet(char* move[], Game* game);

/*
* validate solve command's parameters.
* return value: 1 - validation passed, 0 - otherwise.
*/
int isValidSetParams(char* x, char* y, char* z, Game* game);

/*
* validate validate command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateValidate(char* move[], Game* game);

/*
* validate guess command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateGuess(char* move[], Game* game);

/*
* validate generate command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateGenerate(char* move[], Game* game);

/*
* validate the parameters for a command that receives 2 parameters.
* return value: 1 - validation passed, 0 - otherwise.
*/
int isValidTwoParams(char* x, char* y, int minValue, int maxValue);

/*
* validate undo and redo command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateUndoAndRedo(char* move[], Game* game, int isUndo);

/*
* validate save command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateSave(char* move[], Game* game);

/*
* validate hint and guess hint commands syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateHintAndGuessHint(char* move[], Game* game, int isHint);

/*
* validate autofill command syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateAutofill(char* move[], Game* game);

/*
* validate number solution, exit and reset commands syntax, number of parameters, parameter's type and range.
* return value: 1 - validation passed, 0 - otherwise.
*/
int validateNumSolAndExitAndReset(char* move[], Game* game, Command c, int isExit);
/*---------------------------------------------------------------------------------------*/

/*prints the correct syntaxt of a command*/
void printCommandSyntax(Command c, int maxVal);

#endif
