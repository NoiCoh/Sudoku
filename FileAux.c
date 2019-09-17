/*
 * FileAux:
 * This module handles files, loading and saving board's game.
 */
#include "FileAux.h"

/*
* the function counts the number of elements in the file.
* return value: returns count.
*/
int countInFile(FILE* ptr, int N) {
	int count = 0;
	char* elem = malloc(256 * sizeof(char));
	if (!elem) { /* check if malloc succeseed */
		funcFailed("malloc");
	}
	while (fscanf(ptr, "%s", elem) != EOF) {
		count++;
		if (count > N * N + 2) {
			break;
		}
	}
	rewind(ptr);
	free(elem);
	return count;
}

/*
* the function checks whether the file is empty.
* retuen value: 1 - if file is empty, 0 - otherwise.
*/
int isFileEmpty(FILE* ptr) {
	int size;
	fseek(ptr, 0, SEEK_END);
	size = ftell(ptr);
	if (size == 0) {
		return 1;
	}
	rewind(ptr);
	return 0;
}

/*
* the function checks if the file includes a valid sudoku board (if all of the elements are numbers in the right range
* return value: date struct that includes cell's value and boolean that indicates if the cell is fixed.
*				otherwise return NULL if the input value is invalid.
*/
data* checkInput(char* input, int N) {
	int num, i = 0, fixed = 0;
	data *d;
	d = (data*)calloc(1, sizeof(data));
	if (!d) {
		funcFailed("calloc");
	}
	while (input[i] != '\0') {
		if (!isNum(input[i])) {
			if (input[i] == '.' && input[i + 1] == '\0') {
				fixed = 1;
				break;
			}
			return NULL;
		}
		i++;
	}
	num = atoi(input);
	if (num == 0 && fixed == 1) {
		return NULL;
	}
	if (num < 0 || num > N) {
		return NULL;
	}
	d->num = num;
	d->fixed = fixed;
	return d;
}

/*
* An auxilary function that uploads the board from a given path.
* return value: Board sturct
*/
Board* getUserBoard(Game* game, char* path) {
	int i, j, count, N;
	data* d;
	char m[256], n[256], input[256];
	FILE* ptr;
	blocksize block;
	ptr = fopen(path, "r");
	if (ptr == NULL) {
		printf("Error: File cannot be opened\n");
		return NULL;
	}
	if (isFileEmpty(ptr)) {
		printf("file is empty\n");
		return NULL;
	}
	if (fscanf(ptr, "%s", m) == -1) {
		printf("Error:file format is not valid\n");
		return NULL;
	}
	if (fscanf(ptr, "%s", n) == -1) {
		printf("Error:file format is not valid\n");
		return NULL;
	}
	if (!(isNums(n) && isNums(m))) {
		printf("Error:file format is not valid\n");
		return NULL;
	}
	block.m = atoi(m);
	block.n = atoi(n);
	if (block.n == 0 || block.m == 0) {
		printf("Error:file format is not valid\n");
		return NULL;
	}

	N = block.n * block.m;
	if (N > 99) {
		printf("Error: Block size is too big\n");
		return NULL;
	}

	count = countInFile(ptr, N);
	if (count > N * N || count < N * N) {
		printf("Error:file format is not valid\n");
		return NULL;
	}
	game->board = initialize(block);

	fscanf(ptr, "%s", n);
	fscanf(ptr, "%s", m);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			fscanf(ptr, "%s", input);
			d = checkInput(input, N);
			if (d == NULL) {
				printf("Error:file format is not valid\n");
				return NULL;
			}
			else {
				game->board->cells[i][j].value = d->num;
				game->board->cells[i][j].fixed = d->fixed;
			}
			free(d);
		}
	}

	markErroneousCells(game);
	game->board->erroneous = isBoardErroneous(game->board);

	if (game->mode == solveMode) {
		if (isFixedErrornous(game)) {
			printErroneousBoardError();
			return NULL;
		}
	}

	if (fclose(ptr) == EOF) {
		printf("Error: File was not successfully closed,please try another file\n ");
		return NULL;
	}
	return game->board;
}
