#include "GameAux.h"

/**
 * the function chooses a random value from the options array of a specific cell.
 * when only a single legal value remains, the function returns that value.
 * if there are no options remains -returns 0.
 */
int getRandValue(Game* game, index ind) {
	int sizeOfOptions, val, randNum;
	int* options;
	findOptionsCell(game, ind);
	options = game->board->cells[ind.row][ind.col].options;
	sizeOfOptions = game->board->cells[ind.row][ind.col].optionsSize;
	if (sizeOfOptions == 0) {
		return 0;
	}
	if (sizeOfOptions == 1) {
		val = options[0];
		options[0] = 0;
	}
	else {
		randNum = rand() % (sizeOfOptions);
		val = options[randNum];
		options[randNum] = 0;
		removeZeroFromOptions(game, ind);
	}
	return val;
}

/*
* if the user' command is "edit" with no edditional parameter (path), the user gets an empty 9X9 board
*/
Board* createDefaultBoard() {
	blocksize block;
	Board* board;
	block.m = 3;
	block.n = 3;
	board = initialize(block);
	return board;
}

/*
* if the board is solved, notify the user and enter init mode. else- notify that the board is errornous
* return value: 1 - if board is solved, 0 - otherwise.
*/
int checkIfBoardSolved(Game* game, int isSolveCommand) {
	int N;
	N = game->board->blocksize.m * game->board->blocksize.n;
	if (!IsThereEmptyCell(game->board, N)) {
		if (game->board->erroneous == true) {
			printErroneousBoardError();
			return 0;
		}
		else {
			if (isSolveCommand) {
				printf("This puzzle is already solved. Please load another file or edit this one using edit command\n ");
			}
			else {
				printf("Puzzle solved successfully!\n");
			}
			printBoard(game);
			UpdateGame(game, NULL, initMode);
			return 1;
		}
	}
	return 0;
}

/*
* set value in board's game
*/
void setValue(Game* game, int col, int row, int value) {
	game->board->cells[row][col].value = value;
}
/*
* Count how many empty cells in board's game and returns the count.
*/
int FindHowMuchEmptyCells(Game* game) {
	int m, n, i, j, count, N;
	count = 0;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = n * m;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (game->board->cells[i][j].value == 0) {
				count++;
			}
		}
	}
	return count;
}

/*weighted random selection from the scores array */
int getRandIndex(int numOflegalValues, double* scores) {
	int i;
	double random;
	float sum, accumulative;
	accumulative = 0;
	sum = 0;
	if (numOflegalValues == 1) { /* Only one legal value, so we chose it */
		return 0;
	}
	for (i = 0; i < numOflegalValues; i++) {
		sum += scores[i];
	}

	if (sum == 0) { /* all scores equal to zero then pick randomly */
		random = rand() % numOflegalValues;
		return random;
	}
	for (i = 0; i < numOflegalValues; i++) {
		scores[i] = scores[i] / sum;
	}
	random = ((double)rand()) / ((float)(RAND_MAX)+1); /* random must be in range [0,1] */

	for (i = 0; i < numOflegalValues; i++) {
		accumulative += scores[i];
		if (random < accumulative) {
			return i;
		}
	}
	return 0;
}

/*creates a linked list with all of the board's changes after calling "generate" command.*/
linkedList* createGenerateMoveList(Board* newBoard, Board* orignalBoard) {
	int i, j, row, col, newVal, prevVal, N;
	linkedList* move;
	move = initializeLinkedList();
	row = orignalBoard->blocksize.n;
	col = orignalBoard->blocksize.m;
	N = col * row;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			prevVal = orignalBoard->cells[i][j].value;
			newVal = newBoard->cells[i][j].value;
			if (prevVal != newVal) {
				insertLast(move, i, j, newVal, prevVal);
			}
		}
	}
	return move;
}
/*
* The function counts all legal values's scores for cell <col,row> that are bigger then the threshold
* return value : number of leagal options for cell <col,row>
*/
int getLegalGuess(Game* game, LPsol* lpSol, int row, int col, float threshold, int* legalValues, double* scores) {
	int numOflegalValues, N, val, ix;
	index ind;
	numOflegalValues = 0;
	N = game->board->blocksize.m * game->board->blocksize.n;
	ind.row = row;
	ind.col = col;
	for (val = 1; val <= N; val++) {
		ix = game->board->cells[row][col].ixMap[val - 1] - 1;
		if (ix >= 0) {
			if (lpSol->solBoard[ix] >= threshold) {
				if (isValidOption(game, ind, val, false, false)) {
					scores[numOflegalValues] = lpSol->solBoard[ix];
					legalValues[numOflegalValues++] = val;
				}
			}
		}
	}
	return numOflegalValues;
}
/*
* generate solved board from the Lp solve.
*/
void makeSolBoard(Game* game, LPsol* solve) {
	int i, j, val, var, N;
	N = calculateNfromGame(game);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (val = 1; val <= N; val++) {
				var = (game->board->cells[i][j].ixMap[val - 1] - 1);
				if (var >= 0) {
					if (solve->solBoard[var] == 1) {
						game->board->cells[i][j].value = val;
						break;
					}
				}
			}
		}
	}
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