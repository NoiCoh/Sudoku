#include "Game.h"

/*
* the function response to "solve" command, enters solve mode and upload the board from
* the user's path.
*/
void solveCommand(char* path, Game* game) {
	Board* userBoard;
	int solved;
	game->mode = solveMode;
	userBoard = getUserBoard(game, path);
	UpdateGame(game, userBoard, solveMode);
	if (game->board != NULL) {
		game->userMoves = initializeDoublyLinkedList();
		doublyInsertLast(game->userMoves, NULL);
		game->curMove = game->userMoves->head;
		solved = checkIfBoardSolved(game, 1);
	}
	else {
		UpdateGame(game, userBoard, initMode);
	}
}

/*
* the function counts the number of elements in the file.
*/
int countInFile(FILE *ptr, int N) {
	int count = 0;
	char* elem = malloc(256 * sizeof(char));
	if (!elem) { /* check if malloc succeseed */
		funcFailed("malloc");
	}
	while (fscanf(ptr, "%s",elem) != EOF) {
		count++;
		if (count > N * N + 2 ) { 
			break;
		}
	}
	rewind(ptr);
	return count;
}

/*
* the function checks whether the file is empty.
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
*/
data* checkInput(char* input, int N) {
	int num , i = 0, fixed = 0;
	data* d;
	d = (data*)calloc(1, sizeof(data));
	if (!d) { /* check if malloc succeseed */
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
* an auxilary function that uploads the board from a given path.
*/
Board* getUserBoard(Game* game, char* path) {
	int i, j, count, N;
	data* d;
	char* m;
	char* n;
	char* input;
	FILE* ptr;
	blocksize block;
	input = malloc((256) * sizeof(char));
	n= malloc((256) * sizeof(char));
	m= malloc((256) * sizeof(char));
	if (!(input&&n&&m)) { /* check if malloc succeseed */
		funcFailed("malloc");
	}
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
	if (count > N*N || count < N*N) {
		printf("Error:file format is not valid\n");
		return NULL;
	}

	game->board = initialize(block);
	game->board->blocksize.m = block.m;
	game->board->blocksize.n = block.n;

	fscanf(ptr, "%s", n);
	fscanf(ptr, "%s", m);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			fscanf(ptr, "%s", input);
			d= checkInput(input, N);
			if (d == NULL) {
				printf("Error:file format is not valid\n");
				return NULL;
			}
			else {
				game->board->cells[i][j].value = d->num;
				game->board->cells[i][j].fixed = d->fixed;
			}
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


/*
* the function response to "edit" command, enters edit mode and upload the board from
* the user's path. if no path is given, the function enters edit mode with an empty 9X9 board.
*/
void editCommand(char* path, Game* game) {
	Board* userBoard;
	game->mode = editMode;
	if (path == NULL) {
		userBoard = createDefaultBoard();
	}
	else
	{
		userBoard = getUserBoard(game, path);
	}
	UpdateGame(game, userBoard, editMode);
	if (game->board != NULL) {
		game->userMoves = initializeDoublyLinkedList();
		doublyInsertLast(game->userMoves, NULL);
		game->curMove = game->userMoves->head;
	}
	else {
		UpdateGame(game, userBoard, initMode);
	}
}

/*
* the function response to "mark_errors" command, the x parameter is 1 for display mark errors in the board and 0 otherwise.
*/
void markErrorsCommand(char* input, Game* game) {
	if (strcmp(input, "1") == 0) {
		game->markErrors = true;
	}
	else if (strcmp(input, "0") == 0) {
		game->markErrors = false;
	}
	else {
		printlegalRange("first", "integer", 0, 1);
	}
}

/*
* the function response to "print_board" command and prints the board.
*/
void printCommand(Game* game) {
		printBoard(game);
}


/**
 * the function response to "set" command.
 * set the value val in cell <col,row> according to user's command.
 * in solve mode -if the user sets the last empty cell correctly, the function prints- "Puzzle solved successfully"
 * and enters init mode. if it is an errornous value, the function prints-"Error: The board is erroneous!".
 * the user may set a value into a fixed cell only in edit mode.
 */
int setCommand(Game* game, int col, int row, int val,bool addToMoveList) {
	int prevVal, solved;
	linkedList* move;
	prevVal = game->board->cells[row][col].value;

	/* in solve mode - fixed cells may not be updated */
	if (game->mode == solveMode) {
		if (game->board->cells[row][col].fixed == true) {
			printf("Error: cell is fixed, you can't change this cell\n");
			return 0;
		}
	}
	game->board->cells[row][col].value = val;
	markErroneousCells(game);
	game->board->erroneous = isBoardErroneous(game->board);

	if (val == 0) {/*empty this cell*/
		game->board->cells[row][col].userInput = false;
	}
	else {
		game->board->cells[row][col].userInput = true;
		if (game->mode == solveMode) {
			solved = checkIfBoardSolved(game,0);
			if (solved == 1) {
				return 1;
			}
		}
	}
	
	if (addToMoveList) {
		move = initializeLinkedList();
		insertLast(move, row, col, val, prevVal);
		addMove(game, move);
	}
	return 1;
}

/**
 * the function response to "validate" command.
 * validates that the current state of the board is solvable.
 * solve the generated board with the deterministic backtracking algorithm.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
int validateCommand(Game* game) {
	LPsol* solve;
	if (game->board->erroneous == true) {
		printErroneousBoardError();
		return 0;
	}
	solve = LPsolver(game, true);
	if (solve->solvable == 1) {
		printf("Validation passed: board is solvable\n");
		return 1;
	}
	else {
		printf("Validation failed: board is unsolvable\n");
		return 0;
	}
	return 0;
}

/*
* the function response to "guess" command.
* the function guesses a solution to the current board using LP with a thershold.
* if the board is erroneous, the function prints an error and the command is not executed.
*/
int guessCommand(Game* game, float threshold) {
	int i, j, N, numOflegalValues, * legalValues, randIndex, newVal;
	linkedList* move;
	LPsol* sol;
	double* scores;
	N = game->board->blocksize.m * game->board->blocksize.n;
	if (game->board->erroneous == true) {
		printErroneousBoardError();
		return 0;
	}
	sol = LPsolver(game, false);
	if (sol->solvable == 0) {
		printf("Error: the board is unsolvable\n");
		return 0;
	}
	move = initializeLinkedList();
	legalValues = malloc(N * sizeof(int));
	if (!legalValues) { /* check if malloc succeseed */
		funcFailed("malloc");
	}
	scores = malloc(N * sizeof(double));
	if (!scores) { /* check if malloc succeseed */
		funcFailed("malloc");
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			newVal = 0;
			if (game->board->cells[i][j].value != 0) {
				continue;
			}
			/* get valid options for cell without erroneous*/
			numOflegalValues = getLegalGuess(game, sol, i, j, threshold, legalValues, scores); 

			if (numOflegalValues == 0) { /* if there is no option avaliable for this cell, continue*/
				newVal = 0;
				continue;
			}

			if (legalValues != NULL) {
				randIndex = getRandIndex(numOflegalValues, scores);
				newVal = legalValues[randIndex];
			}
			setValue(game, j, i, newVal);
			insertLast(move, i, j, newVal, 0);
		}
	}
	checkIfBoardSolved(game,0);
	free(legalValues);
	free(scores);
	if (move->size > 0) {
		addMove(game, move);
	}
	else {
		freeList(move);
	}
	return 1;
}

/*
* the function response to "generate" command.
* the function generate a board by randomly filling x empty cells with legal values, running ILP to solve the board, and then clearing all but y random cells .
* if one of the X randomly-chosen cells has no legal value available or the resulting board has no solution- the function reset the board back to the original state
* and repeat previous step. After 1000 such iteratons, treat this as an error in the Soduko genetartor.
*/
void generateCommand(Game* game, int x, int y) {
		int i, t, m, n, N, val, emptyCells, randCol, randRow, var, j;
		index ind;
		linkedList* move;
		Board *orignalBoard, *newBoard;
		LPsol *solve = NULL;
		bool succeedSet, generateSolvableBoard;
		orignalBoard = initialize(game->board->blocksize);
		emptyCells = FindHowMuchEmptyCells(game);
		if (x > emptyCells) {
			printf("Error: Board does not contain %d empty cells\n", x);
			return;
		}
		makeCopyBoard(game->board, orignalBoard); /* save a copy of the orignal board*/
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
				solve = LPsolver(game,true);
				if (solve->solvable == true) {
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

		newBoard = initialize(game->board->blocksize);
		if (y == N*N){/* check if the board solved */
			makeCopyBoard(game->board, newBoard); /*all values in solved board is chosen for the new board*/
			printf("Puzzle solved successfully\n");
			UpdateGame(game, game->board, initMode);
			return;
		}
		else {
			for (i = 0; i < y; i++) {
				randCol = rand() % N;
				randRow = rand() % N;
				if (newBoard->cells[randRow][randCol].value != 0) {
					i--;
					continue;
				}
				else {
					for (j = 1; j <= N;j++) {
						var = (game->board->cells[randRow][randCol].ixMap[j-1] - 1);
						if (var >= 0) {
							if (solve->solBoard[var] == 1) {
								newBoard->cells[randRow][randCol].value = j;
								break;
							}
						}
					}
				}
			}
			makeCopyBoard(newBoard, game->board); /*game board is now the new board that we generate*/
		}
		move = createGenerateMoveList(newBoard, orignalBoard);
		if (move->size > 0) {
			addMove(game, move);
		}
		else {
			freeList(move);
		}
		free(newBoard);
		free(orignalBoard);
}


/* the function adds the user's move to a moves list that tracks user's moves in order to undo\redo them .
* Whenever the user makes a move (via "set,", "autofill", "generate", or "guess"), all items beyond the current move pointer are removed, the new
* move is added to the end of the list and marked as the current move.
*/
void addMove(Game* game, linkedList* move) {
	doublyDeleteAllAfter(game->userMoves, game->curMove);
	doublyInsertLast(game->userMoves, move);
	game->curMove = doublyGetLast(game->userMoves);
}
/*
* the function response to "undo" command.
* undo a previous move done by the user.
*/
int undoCommand(Game* game,bool print) {
	int col, row, newVal, prevVal;
		linkedList* dataToUndo;
		node* i;
		if ((game->curMove==NULL)||(game->curMove == game->userMoves->head)) {
			if (print) {
				printf("Error: No moves to undo\n");
			}
			return 0;
		}
		dataToUndo = game->curMove->move;
		game->curMove = game->curMove->prev;
		i = dataToUndo->head;
		while (i != NULL) {
			col = i->col ;
			row = i->row;
			newVal = i->newVal;
			prevVal = i->prevVal;
			if (print) {
				printf("Undo cell %d,%d: from %d to %d\n", col + 1, row + 1, newVal, prevVal);
			}
			setCommand(game, col, row, prevVal,false);
			i = i->next;
		}
	
	return 1;
}

/*
* the function response to "redo" command.
* redo a move previously undone by the user.
*/
int redoCommand(Game* game) {
	int col, row, newVal, prevVal;
		linkedList* dataToRedo;
		node* i;
		if ((game->curMove == NULL) || (game->curMove->next == NULL)) {
			printf("Error: No moves to redo\n");
			return 0;
		}
		game->curMove = game->curMove->next;
		dataToRedo = game->curMove->move;
		i = dataToRedo->head;
		while ( i != NULL ) {
			col = i->col;
			row = i->row;
			newVal = i->newVal;
			prevVal = i->prevVal;
			printf("Redo cell %d,%d: from %d to %d\n", col + 1, row + 1, prevVal, newVal);
			setCommand(game, col, row, newVal, false);
			i = i->next;
		}

	return 1;
}

/*
* the function response to "save" command and saves the board to a given path.
* errorneous board or board without a solution may not be saved.
*/
void saveGame(Game* game, char* path) {
	FILE* fp;
	int m, n, value, boardSize, i, j;
	char* regCellFormat;
	char* lastCellFormat;
	if (game->mode == editMode) {
		if (game->board->erroneous) {
			printErroneousBoardError();
			return;
		}
		if (!validateCommand(game)) return;
	}
	fp = fopen(path, "w");
	if (fp == NULL) {
		printf("Error: File cannot be opened,please try another file\n");
		return;
	}
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	value = 0;
	boardSize = n * m;
	fprintf(fp, "%d %d\n", m, n);
	if (game->mode == editMode) {
		regCellFormat = "%d. ";
		lastCellFormat = "%d.\n";
	}
	else {
		regCellFormat = "%d ";
		lastCellFormat = "%d\n";
	}
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize - 1; j++) {
			value = game->board->cells[i][j].value;
			if (value == 0) {
				fprintf(fp, "0 ");
			}
			else {
				fprintf(fp, regCellFormat, value);
				value = game->board->cells[i][j].fixed == true;
			}
		}
		value = game->board->cells[i][boardSize - 1].value;
		if (value == 0) {
			fprintf(fp, "0\n");
		}
		else {
			fprintf(fp, lastCellFormat, value);
			value = game->board->cells[i][boardSize - 1].fixed == true;
		}
	}
	if (fclose(fp) == EOF) {
		printf("Error: File was not successfully closed,please try another file\n ");
		return ;
	}
	printf("File saved succussfully!\n");
}
/*
 * the function response to "hint" command and gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the solved board even if it is incorrect for the current state of the board.
 */
void hintCommand(Game* game, char* x, char* y) {
	int row, col, N, i, ix;
	LPsol* sol;
	col = atoi(x) - 1;
	row = atoi(y) - 1;
	N = game->board->blocksize.m * game->board->blocksize.n;
	if (game->board->erroneous == true) {
		printErroneousBoardError();
	}
	else if (game->board->cells[row][col].fixed == true) {
		printf("Error: cell is fixed\n");
	}
	else if (game->board->cells[row][col].value != 0) {
		printf("Error: cell contains a value\n");
	}
	else {
		sol = LPsolver(game, true);
		if (sol->solvable == false) {
			printf("Error: board is unsolvable\n");
		}
		else {
			for (i = 1; i <= N; i++) {
				ix = game->board->cells[row][col].ixMap[i - 1] - 1;
				if (sol->solBoard[ix] == 1) {
					printf("Hint: set cell to %d\n", i);
				}
			}
		}
	}
}

/*
 * the function response to "guess_hint" command.
 * the function gives a guess hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 * hint is given according to the scores Matrix calculated with the LP Solver algorithm.
 */
void guessHintCommand(Game* game, char* x, char* y) {
	int row, col, val, m, n, ix;
	LPsol* sol;
	col = atoi(x) - 1;
	row = atoi(y) - 1;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	if (game->board->erroneous == true) {
		printErroneousBoardError();
	}
	else if (game->board->cells[row][col].fixed == true) {
		printf("Error: cell is fixed\n");
	}
	else if (game->board->cells[row][col].value != 0) {
		printf("Error: cell contains a value\n");
	}
	else {
		sol = LPsolver(game,false);
		if (sol->solvable == false) {
			printf("Error: board is unsolvable\n");
		}
		else {
			for (val = 1; val <= n * m; val++) {
				ix = game->board->cells[row][col].ixMap[val - 1] - 1;
				if (ix >= 0) {
					if (sol->solBoard[ix] != 0) {
						printf("value: %d , score: %f\n", val, sol->solBoard[ix]);
					}
				}
			}
		}
	}
}

/*
 * the function response to "num_solutions" command and prints the number of solutions for the current board.
 * the function uses the exhaustive backtracking algorithm to count all options to solve the board.
 * if the board is errornous it is an error.
 */
int numSolution(Game* game) {
	int count,N;
	Board* copyBoard;
	count= 0;
	copyBoard = initialize(game->board->blocksize);
	makeCopyBoard(game->board, copyBoard);
	N = game->board->blocksize.m * game->board->blocksize.n;
	count = exhaustiveBacktracking(game,N);
	free(copyBoard);
	printf("The number of solutions for the current board: %d\n",count);
	return count;
}

/*
 * the function response to "autofill" command.
 * the function fill cells which contains a single legal value.
 * if the board is errornous it is an error.
 */
void autofillCommand(Game* game) {
	int i, j, m, n, N, val;
	index ind;
	bool res;
	linkedList* move;
	if (game->mode == solveMode) {
		move = initializeLinkedList();
		bool errornous = isBoardErroneous(game->board);
		if (errornous == true) {
			printErroneousBoardError();
		}
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
					res = isValidOption(game, ind, val, true,false);
					if (res == false) {
						game->board->cells[i][j].error = true;
					}
					setValue(game, j, i, val);
					printf("Cell <%d,%d> autofilled to %d\n", j + 1, i + 1, val);
					insertLast(move, i, j, val, 0);
				}
			}
		}
		if (move->size > 0) {
			addMove(game, move);
		}
		else {
			freeList(move);
		}
		checkIfBoardSolved(game,0);
	}
	else {
		if(game->mode==initMode) printErrorMode("init");
		if(game->mode == editMode) printErrorMode("edit");
		
	}
}

/*
* the function response to "reset" command in 'edit' or 'solve' mode.
* the function goes over the entire undo/redo list and revert all moves.
*/
void resetCommand(Game* game) {
		while (game->curMove!=game->userMoves->head) {
			undoCommand(game,false);
		}
}

/**
 * the function prints "Exiting..." , frees all memory resources and terminates the program .
 */
void exiting(Game* game) {
	printf("Exiting...\n");
	if (game->board != NULL) {
		freeBoard(game->board);
	}
	exit(0);
}

/**
 * an auxilary function that free the board memory 
 */
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
