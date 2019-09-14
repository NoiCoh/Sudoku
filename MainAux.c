#include "MainAux.h"
#include "Parser.h"

/**
 * restart the game by starting over with the initialization procedure.
 */
void restart() {
	Game* game;
	printf("Welcome to N&I Sudoku Game!\n");
	game = initializeGame();
	while (1) {
		readUser(game);
	}
}

/*------------------------Error printing------------------------*/

/* prints error and informs the user which parameter is not valid and it's legal range and type */
void printlegalRange(char* param, char* type, int minNum, int maxNum) {
	printf("Error: The %s parameter should be %s in range [%d-%d].\n", param, type, minNum, maxNum);
}
/* prints error if the board is not initilized */
void printBoardNotInit() {
	printf("Error: Board is not initilized\n");
}

/* prints error if the command is unavailable in a specific mode*/
void printErrorMode(char* mode) {
	printf("Error: This command is unavailable in %s mode.\n", mode);
}

/* prints error if the board is erroneous*/
void printErroneousBoardError() {
	printf("Error: The board is erroneous!\n");
}

/*
 * If a standard failure or memory error occurs (failure of malloc, scanf, etc.)
 * the program prints "Error: <function-name> has failed" and exits.
 */
void funcFailed(char* str) {
	printf("Error: %s has failed\n", str);
	exit(0);
}
/*--------------------------------------------------------------*/

/**
 * initialize board values and params to zero and returns the initialized board.
 */
Board* initialize(blocksize block) {
	int i, j, sizeOfRow;
	Cell** arrayBoard;
	Board* board;
	sizeOfRow = block.n * block.m;
	arrayBoard = (Cell**)malloc(sizeOfRow * sizeof(Cell*));
	if (!arrayBoard) {
		funcFailed("malloc");
	}
	for (i = 0;i < sizeOfRow;i++)
	{
		arrayBoard[i] = (Cell *)malloc(sizeof(Cell) * sizeOfRow);
		if (!arrayBoard[i]) {
			funcFailed("malloc");
		}
	}
	for (i = 0;i < sizeOfRow;i++) {
		for (j = 0;j < sizeOfRow;j++) {
			arrayBoard[i][j].value = 0;
			arrayBoard[i][j].fixed = false;
			arrayBoard[i][j].error = false;
			arrayBoard[i][j].userInput = false;
			arrayBoard[i][j].optionsSize = 0;
			arrayBoard[i][j].options = (int *)calloc(sizeOfRow, sizeof(int));
			arrayBoard[i][j].ixMap = (int *)calloc(sizeOfRow, sizeof(int));
			if (!arrayBoard[i][j].options) {
				funcFailed("calloc");
			}
			if (!arrayBoard[i][j].ixMap) {
				funcFailed("calloc");
			}
		}
	}
	board = (Board *)malloc(sizeof(Board));
	if (!board) {
		funcFailed("malloc");
	}
	board->erroneous = false;
	board->cells = arrayBoard;
	board->blocksize.m = block.m;
	board->blocksize.n = block.n;
	return board;
}

/* initialize struct game and allocate memory and returns the initialize game */
Game* initializeGame() {
	Game* game = (Game *)malloc(sizeof(Game));
	if (!game) {
		funcFailed("malloc");
	}
	game->board = NULL;
	game->userMoves=initializeDoublyLinkedList();
	doublyInsertLast(game->userMoves, NULL);
	game->curMove = game->userMoves->head;
	game->mode = initMode;
	game->markErrors = true;
	return game;
}

/*update game's mode or board*/
void UpdateGame(Game* game, Board *userBoard, modes mode){
	game->mode = mode;
	game->board = userBoard;
}

/*
 * Prints the current state of @param board
 */
void printBoard(Game* game) {
	if (game->board != NULL) {
		int i, j, val, size, m, n;
		n = game->board->blocksize.n;
		m = game->board->blocksize.m;
		size = n * m;
		for (i = 0; i < size; i++) {
			if (i % m == 0) {
				printNTimes(4 * size + m + 1);
			}
			for (j = 0; j < size; j++) {
				if (j % n == 0) {
					printf("|");
				}
				val = game->board->cells[i][j].value;
				if (val == 0) {
					printf("    ");
				}
				else {
					printf(" %2d", val);
					if ((game->board->cells[i][j].fixed == true) && (game->mode!= editMode) ) {
						printf(".");
					}
					else if ((game->board->cells[i][j].error == true) && ((game->markErrors == true) || (game->mode == editMode))) {
						printf("*");
					}
					else {
						printf(" ");
					}
				}
			}
			printf("|\n");
		}
		printNTimes(4 * size + m + 1);
	}
}

/*an auxilary function for printing the board*/
void printNTimes(int n) {
	int i;
	for (i = 0; i < n; i++) {
		putchar('-');
	}
	putchar('\n');
}

/**
 * Make a deep copy of @param board to @param copyBorad.
 */
void makeCopyBoard(Board* board, Board* copyBorad) {
	int i, j, m, n, N;
    n= board->blocksize.n;
    m= board->blocksize.m;
	N = n * m;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            copyBorad->cells[i][j].value = board->cells[i][j].value;
            copyBorad->cells[i][j].fixed = board->cells[i][j].fixed;
            copyBorad->cells[i][j].userInput = board->cells[i][j].userInput;
			copyBorad->cells[i][j].error = board->cells[i][j].userInput;
			copyBorad->cells[i][j].options = board->cells[i][j].options;
			copyBorad->cells[i][j].optionsSize = board->cells[i][j].optionsSize;
			copyBorad->cells[i][j].ixMap = board->cells[i][j].ixMap;
        }
    }
	copyBorad->erroneous = board->erroneous;
}

/**
 *check if board has a cell that marked as erroneous.
 */
bool isBoardErroneous(Board* board){
    int n,m,i,j,N;
    m=board->blocksize.m;
    n=board->blocksize.n;
	N = m * n;
    for(i = 0; i < N; i++){
        for (j = 0; j < N ; j++) {
            if(board->cells[i][j].error==true){
                return true;
            }
        }
    }
    return false;
}


/*marks cells which are errornous*/
void markErroneousCells(Game* game) {
	int i, j, n, m, val;
	bool check;
	index ind;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (i = 0; i < m*n; i++) {
		for (j = 0; j < m*n; j++) {
			val = game->board->cells[i][j].value;
			if (val != 0) {
				ind.row = i;
				ind.col = j;
				check = isValidOption(game, ind, val , true,false);
				if (check == true) {
					game->board->cells[i][j].error = false;
				}
				if (check == false) {
					game->board->cells[i][j].error = true;
				}
			}
		}
	}
}

/**
 * the function checks if there is an empty cell in the board.
 * if so, returns true. else, returns false.
 */
bool IsThereEmptyCell(Board* board, int N) {
	int row;
	int col;
	for (row = 0; row < N; row++) {
		for (col = 0; col < N; col++) {
			if (board->cells[row][col].value == 0) {
				return true;
			}
		}
	}
	return false;
}
/**
 * the function checks if @param value is in the same row ,column and box of the cell in @param index.
 * if so returns false. else, returns true.
 * if mark=true we mark erroneous cells, if mark=false we unmark erroneous cells
 * (for example,when setting a cell to zero).
 */
bool isValidOption(Game* game, index ind, int value, bool mark, bool checkOnlyFixed) {
	bool checkBox, checkRowCol;
	index box;
	box = findBoxIndex(game, ind);
	checkBox = checkInBox(game, box, ind, value, mark, checkOnlyFixed);
	checkRowCol = checkInRowAndCol(game, ind, value, mark, checkOnlyFixed);
	if (checkBox && checkRowCol) {
		return true;
	}
	return false;
}

/*
* the function checks if the board contains at least 2 fixed cells with the same value in the same row, colunm or box.
* return value: 1 - if there is an errornous in a fixed cell, 0 - otherwise.
*/
int isFixedErrornous(Game* game) {
	int i, j, n, m, val;
	index ind;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (i = 0; i < m*n; i++) {
		for (j = 0; j < m*n; j++) {
			ind.row = i;
			ind.col = j;
			val = game->board->cells[i][j].value;
			if ((game->board->cells[i][j].error) && (game->board->cells[i][j].fixed)) {
				if (!isValidOption(game, ind, val, true, true)) {
					return 1;
				}
			}
		}
	}
	return 0;
}


/**
 * the function finds the starting cell index of the the block that @param index belongs to.
 */
index findBoxIndex(Game* game, index ind) {
	int m, n;
	index boxIndex;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	boxIndex.col = ind.col - ind.col % n;
	boxIndex.row = ind.row - ind.row % m;
	return boxIndex;
}
/**
 * the function checks if the @param value is in the block starting at @param index block. if mark=true ,
 * the function marks errornous cells in the same block.
 */
bool checkInBox(Game* game, index box, index ind, int value, bool mark, bool checkOnlyFixed) {
	int i, j, valBox, m, n;
	bool res,fixedOk;
	fixedOk = true;
	res = true;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if (((box.row + i) == ind.row) && ((box.col + j) == ind.col)) {
				continue;
			}
			valBox = game->board->cells[box.row + i][box.col + j].value;
			
			if (valBox == value) {
				if (checkOnlyFixed && game->board->cells[box.row + i][box.col + j].fixed) {
					fixedOk = false;
				}
				res = false;
				game->board->cells[box.row + i][box.col + j].error = mark;
			}
		}
	}
	if (checkOnlyFixed) {
		return fixedOk;
	}
	return res;
}
/**
 * the function check if @param value is in the same row and column.
 *  if so returns false. else, returns true. if mark=true ,
 *  the function marks errornous cells in the same row and column.
 */
bool checkInRowAndCol(Game* game, index index, int value, bool mark ,bool checkOnlyFixed) {
	int valRow, valCol, i, n, m;
	bool res, fixedOk;
	res = true;
	fixedOk = true;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (i = 0; i < n * m; i++) {
		if (i != index.row) {
			valCol = game->board->cells[i][index.col].value;
			if (valCol == value) {
				if (checkOnlyFixed && game->board->cells[i][index.col].fixed) {
					fixedOk = false;
				}
				res = false;
				game->board->cells[i][index.col].error = mark;
			}
		}
		if (i != index.col) {
			valRow = game->board->cells[index.row][i].value;
			if (valRow == value) {
				if (checkOnlyFixed && game->board->cells[index.row][i].fixed) {
					fixedOk = false;
				}
				res = false;
				game->board->cells[index.row][i].error = mark;
			}
		}
	}
	if (checkOnlyFixed) {
		return fixedOk;
	}
	return res;
}

/*
* calculate size of a row/col in the sudoku game board from block size and returns it.
*/
int calculateNfromGame(Game* game) {
	int m, n;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	return  n * m;
}

/**
 * an auxilary function that free the board memory
 */
void freeBoard(Board* currentBoard) {
	int k, l, m, n , N;
	m = currentBoard->blocksize.m;
	n = currentBoard->blocksize.n;
	N = n * m;
	if (currentBoard) {
		for (k = 0;k < N;k++) {
			for (l = 0;l < N;l++) {
				if (currentBoard->cells[k][l].options) {
					free(currentBoard->cells[k][l].options);
					currentBoard->cells[k][l].options = NULL;
				}
				if (currentBoard->cells[k][l].ixMap) {
					free(currentBoard->cells[k][l].ixMap);
					currentBoard->cells[k][l].ixMap = NULL;
				}
			}
			if (currentBoard->cells[k])
			{
				free(currentBoard->cells[k]);
			}
		}
		if (currentBoard->cells)
			free(currentBoard->cells);
		free(currentBoard);
	}
}

/**
 * an auxilary function that free the game memory
 */
void freeGame(Game* game) {
	if (game->board) {
		freeBoard(game->board);
	}
	if (game->userMoves) {
		doublyFree(game->userMoves);
	}
	if(game){
		free(game);
	}
}


/**
 * the function creates an array of valid options for a specific cell.
 */
void findOptionsCell(Game* game, index ind) {
	int val, count, m, n;
	count = 0;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (val = 1;val <= n * m;val++) {
		if (isValidOption(game, ind, val, false, false)) {
			game->board->cells[ind.row][ind.col].options[count] = val;
			count++;
		}
	}
	game->board->cells[ind.row][ind.col].optionsSize = count;
}

/**
 * the function removes zero elements from option array.
 */
void removeZeroFromOptions(Game* game, index ind) {
	int i, j, N;
	int* old;
	N = calculateNfromGame(game);
	old = game->board->cells[ind.row][ind.col].options;
	j = 0;
	for (i = 0; i < N; i++) {
		if (old[i] != 0) {
			game->board->cells[ind.row][ind.col].options[j] = old[i];
			j++;
		}
	}
	for (i = j;i < N;i++) {
		game->board->cells[ind.row][ind.col].options[i] = 0;
	}
}
