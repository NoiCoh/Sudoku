#include "Game.h"

Game* initializeGame() {
	Game* game = malloc(sizeof(Game));
	game->mode = init;
	return game;
}

Board* createDefaultBoard()
{
	blocksize block;
	block.m = 3;
	block.n = 3;
	Board* board = initialize(block);
	return board;
}

void UpdateGame(Game* game, Board *userBoard, enum modes mode)
{
	game->mode = mode;
	game->board = userBoard;
}


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
/**
 * set the value z in cell <x,y> (x is the column and y is the row) according to user's command.
 * if the user tries to set a fixed cell, the function prints - "Error: cell is fixed"
 * if the user value is invalid (the value is already in the same box, row or column),
 * the function prints -"Error: value is invalid"
 * if the user set the last empty cell correctly the function prints- "Puzzle solved successfully"
 */
int setCommand(Game *game, char* x, char* y, char* z) {
	int row, col, val, oldVal;
	index ind;
	/**convert string to int**/
	col = atoi(x) - 1;
	row = atoi(y) - 1;
	val = atoi(z);
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
					printBoard(game, false);
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

/**
 *
 *gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 *hint is given according to the solved board, and it is given even if it is incorrect for the current state of the board.
 *
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

/**
 *
 *gives a guess hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 *hint is given according to the scores Matrix calculate with LP Solver
 *
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
			for (i = 0; i < n*m; i++) {
				if (game->scores[col][row][i] != 0) {
					printf("value:%d , score:%f \n", i, game->scores[col][row][i]);
				}
			}
		}
	}
}

/**
 * validates that the current state of the board is solvable.
 * solved the generated board with deterministic backtracking.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
void validateCommand(Game* game) {
	if ((game->mode == solve) || (game->mode == edit)) {
		ILPsol solve = ILPsolver(game);
		if (solve.solvable == true) {
			printf("Validation passed: board is solvable\n");
		}
		else {
			printf("Validation failed: board is unsolvable\n");
		}
	}
	else {
		printErrorMode();
	}
}


/**
 * the function prints "Exiting..." , frees all memory resources and exits.
 */
void exiting(Board* userBoard,Board* board){
    printf("Exiting...\n");
	freeBoard(board);
    freeBoard(userBoard);
   
    exit(0);

}

void freeBoard(Board *currentBoard) {
	int k,l,m,n;
	m=currentBoard->blocksize.m;
	n=currentBoard->blocksize.n;
	if(currentBoard){
		for(k=0;k<n;k++) {
			for(l=0;l<m;l++) {
				if(currentBoard->cells[k][l].options) {
					free(currentBoard->cells[k][l].options);
					currentBoard->cells[k][l].options = NULL;
				}
			}
			if(currentBoard->cells[k])
			{
				free(currentBoard->cells[k]);
			}
		}
		if(currentBoard->cells)
			free(currentBoard->cells);
		free(currentBoard);
	}
}

void solveCommand(char* path,Game* game) {
	Board* userBoard;
	userBoard = getUserBoard(game,path);
	UpdateGame(game,userBoard, solve, false);
}

Board* getUserBoard(Game* game ,char* path) {
	int j, i = 0, k = 0;
	blocksize block;
	FILE *ptr;
	char *delim = " \n";
	ptr = fopen(path, "r");
	if (ptr == NULL) {
		printf("Error: File cannot be opened\n");
	}
	else {
		block.m = fgetc(ptr) - '0';
		fgetc(ptr);
		block.n = fgetc(ptr) - '0';
		const int boardSize = block.n * block.m;
		game->board = initialize(block);
		game->board->blocksize.m = block.m;
		game->board->blocksize.n = block.n;
		char **input = calloc(boardSize, sizeof(char*));
		char *row = calloc(4096, sizeof(char));
		fgets(row, 4096, ptr);
		while (fgets(row, 4096, ptr) != EOF && row != NULL && *row!='\n') {
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
		userBoard = getUserBoard(game,path);
	}
	UpdateGame(game, userBoard, edit, true);
	printBoard(game);
}
void markErrorsCommand(int x, Game * game) {
	if (game->mode == solve ) {
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

void printCommand(Game * game) {
	if (game->mode == solve || game->mode == edit) {
		printBoard(game);
	}
	else {
		printErrorMode();
	}
}
