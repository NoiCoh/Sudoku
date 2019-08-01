#include "Game.h"

/**
 * initialize board values and params to zero.
 */
Board* initialize(blocksize block){
    int i,j,sizeOfRow;
    Cell **arrayBoard;
    Board* board;
    sizeOfRow = block.n*block.m;
    arrayBoard = malloc(sizeOfRow*sizeof(Cell *));
    if(!arrayBoard){
        funcFailed("malloc");
    }
    for(i=0;i<sizeOfRow;i++)
    {
        arrayBoard[i] = malloc(sizeof(Cell)*sizeOfRow);
        if(!arrayBoard[i]){
            funcFailed("malloc");
        }
    }
    for(i=0;i<sizeOfRow;i++){
        for(j=0;j<sizeOfRow;j++){
            arrayBoard[i][j].value = 0;
            arrayBoard[i][j].fixed = false;
            arrayBoard[i][j].error = false;
            arrayBoard[i][j].userInput = false;
            arrayBoard[i][j].optionsSize = 0;
            arrayBoard[i][j].options = calloc(sizeOfRow, sizeof(int));
            if(!arrayBoard[i][j].options){
                funcFailed("malloc");
            }
        }
    }
    board = malloc(sizeof(Board));
    if(!board){
        funcFailed("malloc");
    }
	board->erroneous = false;
    board->cells = arrayBoard;
    board->solved=false;
    board->blocksize.m=block.m;
    board->blocksize.n=block.n;
    return board;
}

/**
 * the function returns user board with random fixed cells from the solution board and empty cells.
 * the number of fixed cell is according to the user's input (@param hint).
 */
Board* makeUserBoard(Board* solvedBoard,int hint,blocksize block){
    int count, x, y;
    Board* userBoard;
    count =0;
    userBoard = initialize(block);
    while(count<hint){
        x= rand()%9;
        y= rand()%9;
        if(userBoard->cells[y][x].fixed==false){
            userBoard->cells[y][x].value=solvedBoard->cells[y][x].value;
            userBoard->cells[y][x].fixed=true;
            count++;
        }
    }
    return userBoard;
}

/*
* the function response to "solve" command, move to solve state and upload board
*/
void solveCommand(char* path, Game* game) {
	Board* userBoard;
	userBoard = getUserBoard(game, path);
	UpdateGame(game, userBoard, solve);
	printBoard(game);
}

/*
* upload board from path
*/
Board* getUserBoard(Game* game, char* path) {
	int j, i = 0, k = 0;
	blocksize block;
	char* delim = " \n";
	game->ptr = fopen(path, "r");
	if (game->ptr == NULL) {
		printf("Error: File cannot be opened\n");
	}
	else {
		block.m = fgetc(game->ptr) - '0';
		fgetc(game->ptr);
		block.n = fgetc(game->ptr) - '0';
		const int boardSize = block.n * block.m;
		game->board = initialize(block);
		game->board->blocksize.m = block.m;
		game->board->blocksize.n = block.n;
		char** input = calloc(boardSize, sizeof(char*));
		char* row = calloc(4096, sizeof(char));
		fgets(row, 4096, game->ptr);
		while (fgets(row, 4096, game->ptr) != EOF && row != NULL && *row != '\n' && i < boardSize) {
			input[k] = strtok(row, delim);
			while (input[k] != NULL) {
				k++;
				input[k] = strtok(NULL, delim);
			}
			for (j = 0; j < boardSize; j++) {
				if (strlen(input[j]) == 1) {
					int x = atoi(input[j]);
					game->board->cells[i][j].value = x;
				}
				else {
					input[j][1] = '\0';
					int x = atoi(input[j]);
					game->board->cells[i][j].value = x;
					game->board->cells[i][j].fixed = true;
				}
			}
			i++;
			k = 0;
		}
		return game->board;
	}
}

void editCommand(char* path, Game* game) {
	Board* userBoard;
	if (path == NULL) {
		userBoard = createDefaultBoard();
	}
	else
	{
		userBoard = getUserBoard(game, path);
	}
	UpdateGame(game, userBoard, edit, true);
	game->mode = edit;
	printBoard(game);
}

/*
* the function response to "mark_errors" command, the x parameter is 1 for display mark errors in the board and 0 otherwise.
*/
void markErrorsCommand(int x, Game* game) {
	if (game->mode == solve) {
		if (x == 1) {
			game->markErrors = true;
			printBoard(game);
		}
		if (x == 0) {
			game->markErrors = false;
			printBoard(game);
		}
	}
	else {
		printErrorMode();
	}
}

/*
* the function response to "print_board" command and prints the board.
*/
void printCommand(Game* game) {
	if (game->mode == solve || game->mode == edit) {
		printBoard(game);
	}
	else {
		printErrorMode();
	}
}


/**
 * set the value val in cell <row,col> according to user's command.
 * if the user tries to set a fixed cell, the function prints - "Error: cell is fixed"
 * if the user value is invalid (the value is already in the same box, row or column),
 * the function prints -"Error: value is invalid"
 * if the user set the last empty cell correctly the function prints- "Puzzle solved successfully"
 */
int setCommand(Game *game, int row, int col, int val) {
	int oldVal;
	index ind;
	ind.col = col;
	ind.row = row;
	if ((game->mode == solve) || (game->mode == edit)) {
		if (game->mode == solve) {
			if (game->board->cells[row][col].fixed == true) {
				printf("Error: cell is fixed\n");
				printBoard(game);
				return 0;
			}
			if (!IsThereEmptyCell(game->board)) {
				if (game->board->erroneous == true) {
					printf("Error: The solution is erroneous");
					printBoard(game);
				}
				else {
					printf("Puzzle solved successfully\n");
					printBoard(game);
					game->board->solved = true;
					game->mode = init;
				}
			}
		}
		if (val == 0) {
			oldVal = game->board->cells[row][col].value;
			if (game->board->cells[row][col].error == true) {
				game->board->cells[row][col].error = false;
				isValidOption(game, ind, oldVal, false);
				game->board->erroneous = isBoardErroneous(game->board);
			}
			game->board->cells[row][col].value = val;
			game->board->cells[row][col].userInput = false;
			printBoard(game);
		}
		else if (!isValidOption(game, ind, val, true)) {
			game->board->cells[row][col].error = true;
			game->board->erroneous = true;
		}
		game->board->cells[row][col].value = val;
		game->board->cells[row][col].userInput = true;
		printBoard(game);
		return 1;
	}
	else {
	printErrorMode();
	return 1;
	}
}

/**
 * validates that the current state of the board is solvable.
 * solved the generated board with deterministic backtracking.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
int validateCommand(Game* game) {
	if ((game->mode == solve) || (game->mode == edit)) {
		ILPsol solve = ILPsolver(game);
		if (solve.solvable == true) {
			printf("Validation passed: board is solvable\n");
			return 1;
		}
		else {
			printf("Validation failed: board is unsolvable\n");
			return 0;
		}
	}
	else {
		printErrorMode();
		return 0;
	}
}

/*
* the function response to "generate" command.
* the function generate a board by randomly choose x cells to fill and solved to board and then fill only y cells and empty all other cells
* if one of the X randomly-chosen cells has no legal value available or the resulting board has no solution the function reset the board back to the original state
* and repeat previous step. After 1000 such iteratons, treat this as an error in the puzzle genetartor.
*/
int generate(Game* game, int x, int y) {
	if (game->mode = edit) {
		int i, t, m, n, N, val, emptyCells, randCol, randRow;
		index ind;
		Board* orignalBoard;
		ILPsol solve;;
		bool succeedSet, generateSolvableBoard;
		emptyCells = FindHowMuchEmptyCells(game);
		if (x > emptyCells) {
			printf("Error: Board does not contain %d empty cells\n", x);
		}
		makeCopyBoard(game->board, orignalBoard);
		m = game->board->blocksize.m;
		n = game->board->blocksize.n;
		N = n * m;
		for (t = 0; t < 1000; t++) {
			succeedSet = true;
			generateSolvableBoard = false;
			for (i = 0; i < x; i++) {
				ind.col = rand() % N;
				ind.row = rand() % N;
				if (game->board->cells[ind.row][ind.col].value != 0) {
					i--;
					continue;
				}
				findOptionsCell(game, ind);
				val = getRandValue(game->board, ind);
				if (val == 0) { /*There is no valid value for this cell*/
					makeCopyBoard(orignalBoard, game->board);
					succeedSet = false;
					break;
				}
				else {
					setValue(game, ind.col, ind.row, val);
				}
			}
			if (succeedSet) {
				solve = ILPsolver(game);
				if (solve.solvable == true) {
					generateSolvableBoard = true;
					break;
				}
				else {
					makeCopyBoard(orignalBoard, game->board);
					continue;
				}
			}
		}
		if (!generateSolvableBoard) {
			makeCopyBoard(orignalBoard, game->board);
			printf("Error: Can't generate solvable board with parameters %d and %d \n", x, y);
			return;
		}
		Board* newBoard;
		newBoard = initialize(game->board->blocksize);
		for (i = 0; i < y; i++) {
			randCol = rand() % N;
			randRow = rand() % N;
			if (newBoard->cells[randRow][randCol].value != 0) {
				y--;
				continue;
			}
			else {
				newBoard->cells[randRow][randCol].value = solve.solBoard->cells[randRow][randCol].value;
			}
		}
		makeCopyBoard(newBoard, game->board);
	}
	else {
		printErrorMode();
	}
}

/* Whenever the user makes a move (via "set,", "autofill", "generate", or "guess"), the redo
* part of the list is cleared, i.e., all items beyond the current pointer are removed, the new
* move is added to the end of the list and marked as the current move, i.e., the pointer is
* updated to point to it.
*/
void addMove(Game* game, linkedList* move) {
	doublyDeleteAllAfter(game->userMoves, game->curMove);
	doublyInsertLast(game->userMoves, move);
	game->curMove = doublyGetLast(game->userMoves);
}

int undoCommand(Game* game) {
	if ((game->mode = solve) || (game->mode == edit)) {
		linkedList* dataToUndo;
		node* i;
		if (game->curMove == NULL) {
			printf("Error: No moves to undo");
			return 0;
		}
		dataToUndo = game->curMove->move;
		game->curMove = game->curMove->prev;
		for (i = dataToUndo->head; i != NULL; i = i->next) {
			printf("Undo cell %d,%d: from %d to %d\n", i->row + 1, i->col + 1, i->newVal, i->prevVal);
			setCommand(&game, i->row, i->col, i->prevVal);
		}
	}
	else {
		printErrorMode();
	}
	return 1;
}

int redoCommand(Game* game) {
	if ((game->mode = solve) || (game->mode == edit)) {
		linkedList* dataToRedo;
		node* i;
		if ((game->curMove == NULL) || (game->curMove->next == NULL)) {
			printf("Error: No moves to redo");
			return 0;
		}
		game->curMove = game->curMove->next;
		dataToRedo = game->curMove->move;

		for (i = dataToRedo->head; i != NULL; i = i->next) {
			printf("Redo cell %d,%d: from %d to %d\n", i->row + 1, i->col + 1, i->prevVal, i->newVal);
			setCommand(&game, i->row, i->col, i->newVal);
		}
	}
	else {
		printErrorMode();
	}
	return 1;
}

/*
* the function response to "save" command and save the board to a path.
* errorneous board or board without a solution may not be saved.
*/
void saveGame(Game* game, char* path) {
	if (game->mode == init) {
		printErrorMode();
		return;
	}
	if (game->mode == edit) {
		if (game->board->erroneous) {
			printf("Error: The board is erroneous ");
			return;
		}
		if (!validateCommand(game)) return;
	}
	FILE* fp;
	fp = fopen(path, "w");
	int m = game->board->blocksize.m;
	int n = game->board->blocksize.n;
	int value = 0;
	const int boardSize = n * m;
	fprintf(fp, "%d %d\n", m, n);
	char* regCellFormat;
	char* lastCellFormat;
	if (game->mode == edit) {
		regCellFormat = "%d. ";
		lastCellFormat = "%d.\n";
	}
	else {
		regCellFormat = "%d ";
		lastCellFormat = "%d\n";
	}
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize - 1; j++) {
			value = game->board->cells[i][j].value;
			if (value == 0) {
				fprintf(fp, "0 ");
			}
			else {
				fprintf(fp, regCellFormat, value);
			}
		}
		value = game->board->cells[i][boardSize - 1].value;
		if (value == 0) {
			fprintf(fp, "0\n");
		}
		else {
			fprintf(fp, lastCellFormat, value);
		}
	}
	fclose(fp);
}
/*
 * gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the solved board, and it is given even if it is incorrect for the current state of the board.
 */
void hintCommand(Game* game, char* x, char* y) {
	int row, col, z;
	col = atoi(x) - 1;
	row = atoi(y) - 1;
	ILPsol sol;
	if (game->board->erroneous == true) {
		printf("Error: board is erroneous\n");
	}
	else if (game->board->cells[row][col].fixed == true) {
		printf("Error: cell is fixed\n");
	}
	else if (game->board->cells[row][col].value != 0) {
		printf("Error: cell contain a value\n");
	}
	else {
		sol = ILPsolver(game);
		if (sol.solvable == false) {
			printf("Error: board is unsolvable\n");
		}
		else {
			z = game->solBoard->cells[row][col].value;
			printf("Hint: set cell to %d\n", z);
		}
	}
}

/*
 * gives a guess hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the scores Matrix calculate with LP Solver
 */
void guessHintCommand(Game* game, char* x, char* y) {
	int row, col, z, check, i, m, n;
	col = atoi(x) - 1;
	row = atoi(y) - 1;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	if (game->board->erroneous == true) {
		printf("Error: board is erroneous\n");
	}
	else if (game->board->cells[row][col].fixed == true) {
		printf("Error: cell is fixed\n");
	}
	else if (game->board->cells[row][col].value != 0) {
		printf("Error: cell contain a value\n");
	}
	else {
		check = LPsolver(game);
		if (check == 0) {
			printf("Error: board is unsolvable\n");
		}
		else {
			for (i = 0; i < n * m; i++) {
				if (game->scores[col][row][i] != 0) {
					printf("value:%d , score:%f \n", i, game->scores[col][row][i]);
				}
			}
		}
	}
}

void autofillCommand(Game* game) {
	if (game->mode == solve) {
		bool errornous = isBoardErroneous(game->board);
		if (errornous == true) {
			printf("Error: board is erroneous\n");
		}
		int i, j, m, n, N, val;
		index ind;
		bool res;
		n = game->board->blocksize.n;
		m = game->board->blocksize.m;
		N = n * m;
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				ind.row = i;
				ind.col = j;
				findOptionsCell(game, ind);
			}
		}
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				ind.row = i;
				ind.col = j;
				if ((game->board->cells[i][j].optionsSize == 1) && (game->board->cells[i][j].value == 0)) {
					val = game->board->cells[i][j].options[0];
					res = isValidOption(game, ind, val, true);
					if (res == false) {
						game->board->cells[i][j].error = true;
					}
					game->board->cells[i][j].value = val;
					printf("Cell <%d,%d> autofilled to %d\n", i + 1, j + 1, val);

				}
			}
		}
		printBoard(game);

	}
	else {
		printErrorMode();
	}
}
/**
 * the function prints "Exiting..." , frees all memory resources and exits.
 */
void exiting(Game* game) {
	printf("Exiting...\n");
	freeBoard(game->board);
	fclose(game->ptr);
	exit(0);

}

void freeBoard(Board* currentBoard) {
	int k, l, m, n;
	m = currentBoard->blocksize.m;
	n = currentBoard->blocksize.n;
	if (currentBoard) {
		for (k = 0;k < n;k++) {
			for (l = 0;l < m;l++) {
				if (currentBoard->cells[k][l].options) {
					free(currentBoard->cells[k][l].options);
					currentBoard->cells[k][l].options = NULL;
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
 * check the user value during setCommand.
 * check if the value is invalid (if the value is already in the same box, row or column).
 * if the value is valid returns true, else returns false.
 */
 /**
bool isValidSet(Board* userBoard,index ind, int val){
	int i,value;
	findOptionsCell(userBoard,ind);
	value=userBoard->cells[ind.row][ind.col].value;
	if(value!=0){
		userBoard->cells[ind.row][ind.col].options[userBoard->cells[ind.row][ind.col].optionsSize]= value;
	}
	for(i=0;i<9;i++){
	  if(userBoard->cells[ind.row][ind.col].options[i]==val){
		  return true;
	  }
	}
	return false;
}**/