#include "Game.h"

/*
* the function response to "solve" command, enters solve mode and upload the board from
* the user's path.
*/
void solveCommand(char* path, Game* game) {
	Board* userBoard = NULL;
	game->mode = solveMode;
	if (game->board != NULL) {
		freeBoard(game->board);
	}
	if (path != NULL) {
		userBoard = getUserBoard(game, path);
	}
	UpdateGame(game, userBoard, solveMode);
	if (game->board != NULL) {
		doublyFree(game->userMoves);
		game->userMoves = initializeDoublyLinkedList();
		doublyInsertLast(game->userMoves, NULL);
		game->curMove = game->userMoves->head;
		checkIfBoardSolved(game, 1);
	}
	else {
		UpdateGame(game, userBoard, initMode);
	}
}

/*
* the function response to "edit" command, enters edit mode and upload the board from
* the user's path. if no path is given, the function enters edit mode with an empty 9X9 board.
*/
void editCommand(char* path, Game* game) {
	Board* userBoard;
	if (game->board != NULL) {
		freeBoard(game->board);
	}
	if (path == NULL) {
		userBoard = createDefaultBoard();
	}
	else{
		userBoard = getUserBoard(game, path);
	}
	UpdateGame(game, userBoard, editMode);
	if (game->board != NULL) {
		doublyFree(game->userMoves);
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

/*
 * the function response to "set" command.
 * set the value val in cell <col,row> according to user's command.
 * in solve mode -if the user sets the last empty cell correctly, the function prints- "Puzzle solved successfully"
 * and enters init mode. if it is an errornous value, the function prints-"Error: The board is erroneous!".
 * the user may set a value into a fixed cell only in edit mode.
 */
void setCommand(Game* game, int col, int row, int val,bool addToMoveList) {
	int prevVal, solved;
	linkedList* move;
	prevVal = game->board->cells[row][col].value;

	/* in solve mode - fixed cells may not be updated */
	if (game->mode == solveMode) {
		if (game->board->cells[row][col].fixed == true) {
			printf("Error: cell is fixed, you can't change this cell\n");
			return;
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
				return;
			}
		}
	}
	if (addToMoveList) {
		move = initializeLinkedList();
		insertLast(move, row, col, val, prevVal);
		addMove(game, move);
	}
}

/**
 * the function response to "validate" command.
 * validates that the current state of the board is solvable.
 * solve the generated board with the Linear programming algorithm.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 * return value: 1 - if validation passed, 0 - otherwise.
 */
int validateCommand(Game* game,int isSave) {
	LPsol* solve;
	if (game->board->erroneous == true) {
		printErroneousBoardError();
		return 0;
	}
	solve = LPsolver(game, true);
	if (solve->solvable == 1) {
		if (!isSave) {
			printf("Validation passed: board is solvable\n");
		}
		freeLpSol(solve);
		return 1;
	}
	else {
		if (!isSave) {
			printf("Validation failed: board is unsolvable\n");
		}
		return 0;
	}
	return 0;
}

/*
* the function response to "guess" command.
* the function guesses a solution to the current board using LP with a thershold.
* if the board is erroneous, the function prints an error and the command is not executed.
* return value: 1 - guess command succeeded , 0 - otherwise.
*/
int guessCommand(Game* game, float threshold) {
	int i, j, N, numOflegalValues, * legalValues, randIndex, newVal;
	linkedList* move;
	LPsol* lpSol;
	double* scores;
	N = game->board->blocksize.m * game->board->blocksize.n;
	if (game->board->erroneous == true) {
		printErroneousBoardError();
		return 0;
	}
	lpSol = LPsolver(game, false);
	if (lpSol->solvable == 0) {
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
			numOflegalValues = getLegalGuess(game, lpSol, i, j, threshold, legalValues, scores); 

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
	freeLpSol(lpSol);
	if (move->size > 0) {
		addMove(game, move);
	}
	else {
		freeList(move);
	}
	return 1;
}

/*
* the function only excute in EDIT mode.
* the function response to "generate" command.
* the function generates a board by randomly filling x empty cells with legal values, running ILP to solve the board, and then clearing all but y random cells .
* if one of the X randomly-chosen cells has no legal value available or the resulting board has no solution- the function reset the board back to the original state
* and repeat previous step. After 1000 such iteratons, treat this as an error in the Soduko genetartor.
*/
void generateCommand(Game* game, int x, int y) {
	int i, t, N, val, emptyCells, randCol, randRow;
		index ind;
		linkedList* move;
		Board *orignalBoard, *newBoard;
		LPsol *lpSol = NULL;
		bool succeedSet, generateSolvableBoard;
		orignalBoard = initialize(game->board->blocksize);
		emptyCells = FindHowMuchEmptyCells(game);/*checks if we have enough empty cells*/
		if (x > emptyCells) {
			printf("Error: Board does not contain %d empty cells\n", x);
			return;
		}
		
		makeCopyBoard(game->board, orignalBoard);/* save a copy of the orignal board*/
		N = calculateNfromGame(game);
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
				val = getRandValue(game, ind);
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
				lpSol = LPsolver(game,true);
				if (lpSol->solvable == 1) {
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
			markErroneousCells(game);
			printf("Error: Can't generate solvable board with parameters %d and %d \n", x, y);
			return;
		}
		
		makeSolBoard(game,lpSol);
		/* make a new board with only y cells filled*/
		newBoard = initialize(game->board->blocksize);
		for (i = 0; i < y; i++) {
			randCol = rand() % N;
			randRow = rand() % N;
			if (newBoard->cells[randRow][randCol].value != 0) {
				i--;
				continue;
			}
			newBoard->cells[randRow][randCol].value = game->board->cells[randRow][randCol].value;
		}
		makeCopyBoard(newBoard, game->board); /*game board is now the new board that we generate*/
		markErroneousCells(game);
		move = createGenerateMoveList(newBoard, orignalBoard);
		if (move->size > 0) {
			addMove(game, move);
		}
		else {
			freeList(move);
		}
		freeLpSol(lpSol);
		free(newBoard);
		free(orignalBoard);
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
			printf("File was not saved\n");
			return;
		}
		if (!validateCommand(game,1)) {
			printf("board is unsolvable\n");
			printf("File was not saved\n");
			return;
		}
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
	LPsol* lpSol;
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
		lpSol = LPsolver(game, true);
		if (lpSol->solvable == 0 ) {
			printf("Error: board is unsolvable\n");
		}
		else {
			for (i = 1; i <= N; i++) {
				ix = game->board->cells[row][col].ixMap[i - 1] - 1;
				if (ix >= 0) {
					if (lpSol->solBoard[ix] == 1) {
						printf("Hint: set cell to %d\n", i);
					}
				}
			}
			freeLpSol(lpSol);
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
	LPsol* lpSol;
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
		lpSol = LPsolver(game,false);
		if (lpSol->solvable == false) {
			printf("Error: board is unsolvable\n");
		}
		else {
			for (val = 1; val <= n * m; val++) {
				ix = game->board->cells[row][col].ixMap[val - 1] - 1;
				if (ix >= 0) {
					if (lpSol->solBoard[ix] != 0) {
						printf("value: %d , score: %f\n", val, lpSol->solBoard[ix]);
					}
				}
			}
		}
		freeLpSol(lpSol);
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
			return;
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

/*
* the function response to "exit" command.
* the function prints "Exiting..." , frees all memory resources and terminates the program .
*/
void exiting(Game* game) {
	printf("Exiting...\n");
	if (game != NULL) {
		freeGame(game);
	}
	exit(0);
}
