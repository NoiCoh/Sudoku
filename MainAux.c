#include "MainAux.h"

void printErrorMode() {
	printf("Error: The command is not available ");
}

void printWelcome() {
	printf("Welcome to N&I Sudoku Game!\n ");
}

void printErroneousBoardError() {
	printf("Error: The board is erroneous\n");
}

void printNTimes(int n) {
	int i;
	for (i = 0; i < n; i++) {
		putchar('-');
	}
	putchar('\n');
}

Board* createDefaultBoard()
{
	blocksize block;
	block.m = 3;
	block.n = 3;
	Board* board = initialize(block);
	return board;
}

Game* initializeGame() {
	Game* game = malloc(sizeof(Game));
	game->userMoves=initializeDoublyLinkedList();
	game->curMove = NULL;
	game->mode = init;
	game->markErrors = true;
	return game;
}

void UpdateGame(Game* game, Board *userBoard, enum modes mode)
{
	game->mode = mode;
	game->board = userBoard;
}


/**
 * Prints the current state of @param board
 */
void printBoard(Game* game) {
    int i, j, val, size, m, n;
    n = game->board->blocksize.n;
    m = game->board->blocksize.m;
    size = n * m;
    for (i = 0; i < size; i++) {
        if (i % n == 0) {
            printNTimes(4*size+m+1);
        }
        for (j = 0; j < size; j++) {
            if (j % m == 0) {
                printf("|");
            }
            val = game->board->cells[i][j].value;
            if (val == 0) {
                printf("    ");
            } else {
                printf(" %2d", val);
                if (game->board->cells[i][j].fixed == true) {
                    printf(".");
                } else if ((game->board->cells[i][j].error == true)&&((game->markErrors==true)||(game->mode==edit ))){
                    printf("*");
                } else {
                    printf(" ");
                }
            }
        }
        printf("|\n");
    }
    printNTimes(4*size+m+1);
}


/**
 * Make a deep copy of @param board to @param copyBorad.
 */
void makeCopyBoard(Board* board, Board* copyBorad) {
    int i, j, m ,n;
    n= board->blocksize.n;
    m= board->blocksize.m;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            copyBorad->cells[i][j].value = board->cells[i][j].value;
            copyBorad->cells[i][j].fixed = board->cells[i][j].fixed;
            copyBorad->cells[i][j].userInput = board->cells[i][j].userInput;
        }
    }
}
/**
 * If a standard failure or memory error occurs (failure of malloc, scanf, etc.)
 * the program prints "Error: <function-name> has failed" and exits.
 */
void funcFailed(char* str){
    printf("Error: %s has failed\n", str);
    exit(0);
}
/**
 *check if board has cell that mark as erroneous
 */
bool isBoardErroneous(Board* board){
    int n,m,i,j;
    m=board->blocksize.m;
    n=board->blocksize.n;
    for(i = 0; i < n; i++){
        for (j = 0; j < m ; j++) {
            if(board->cells[i][j].error==true){
                return true;
            }
        }
    }
    return false;
}

/**
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF(){
    if(feof(stdin)){
        printf("Exiting...\n");
        exit(0);
    }
}
/*
* set value in board's game
*/
void setValue(Game* game, int col, int row, int value) {
	game->board->cells[row][col].value = value;
}

/*
Count how many empty cells in board's game
**/
int FindHowMuchEmptyCells(Game* game) {
	int m, n, i, j, count;
	count = 0;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			if (game->board->cells[i][j].value == 0) {
				count++;
			}
		}
	}
	return count;
}

int getLegalGuess(Game* game, int row, int col, float threshold, int* legalValues) {
	int numOflegalValues, N, val;
	index ind;
	numOflegalValues = 0;
	N = game->board->blocksize.m * game->board->blocksize.n;
	ind.col = col;
	ind.row = row;
	for (val = 0; val < N; val++) {
		if (game->scores[row][col][val] >= threshold) {
			if (isValidOption(game, ind, val, false)) {
				legalValues[numOflegalValues++] = val;
			}
		}
	}
	return numOflegalValues;
}

float* getScoresOfLegalValue(Game* game, int row, int col, int numOflegalValues, int* legalValues) {
	int i, val;
	float* scores;
	for (i = 0; i < numOflegalValues; i++) {
		val = legalValues[i];
		scores[i] = game->scores[row][col][val];
	}
	return scores;
}

int getRandIndex(Game* game, int numOflegalValues, float* scores) {
	int i, random;
	float sum, accumulative;
	if (numOflegalValues == 1) {
		return 0;
	}
	for (i = 0; i < numOflegalValues; i++) {
		sum += scores[i];
	}
	if (sum == 0) { //all scores equal to zero then pick randomly
		random = rand() % numOflegalValues;
		return random;
	}
	for (i = 0; i < numOflegalValues; i++) {
		scores[i] = scores[i] / sum;
	}
	random = (double)rand() / (float)(RAND_MAX)+1;

	for (i = 0; i < numOflegalValues; i++) { 
		accumulative += scores[i]; 
		if (random < accumulative) {
			return i;
		}
	}
	return 0;
}


