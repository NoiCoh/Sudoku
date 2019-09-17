/*
 * FileAux:
 * This module handle files, loading and saving board's game.
 */
#ifndef FILEAUX_H
#define FILEAUX_H
#include "MainAux.h"
#include "ParserAux.h"

/*
* the function counts the number of elements in the file.
* return value: returns count.
*/
int countInFile(FILE* ptr, int N);

/*
* the function checks whether the file is empty.
* retuen value: 1 - if file is empty, 0 - otherwise.
*/
int isFileEmpty(FILE* ptr);

/*
* the function checks if the file includes a valid sudoku board (if all of the elements are numbers in the right range
* return value: date struct that includes cell's value and boolean that indicates if the cell is fixed.
*				otherwise return NULL if the input value is invalid.
*/
data* checkInput(char* input, int N);

/*
* An auxilary function that uploads the board from a given path.
* return value: Board sturct
*/
Board* getUserBoard(Game* game, char* path);


#endif
