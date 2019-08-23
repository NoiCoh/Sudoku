#include "MainAux.h"

void printExtraParams() {
	printf("Error: Too many parameters.\n");
}

void printlegalRange(char* param, char* type,int minNum,int maxNum) {
	printf("Error: The %s parameter should be %s in range [%d-%d].\n",param,type,minNum, maxNum);
}

void printBoardNotInit() {
	printf("Error: Board is not initilize\n");
}

void printErrorMode(char* mode) {
	printf("Error: This command is unavailable in %s mode.\n",mode);
}

void printWelcome() {
	printf("Welcome to N&I Sudoku Game!\n");
}

void printErroneousBoardError() {
	printf("Error: The board is erroneous!\n");
}

void printNTimes(int n) {
	int i;
	for (i = 0; i < n; i++) {
		putchar('-');
	}
	putchar('\n');
}

Board* createDefaultBoard(){
	blocksize block;
	Board* board;
	block.m = 3;
	block.n = 3;
	board = initialize(block);
	return board;
}

int checkIfBoardSolved(Game* game) {
	int N;
	N = game->board->blocksize.m * game->board->blocksize.n;
	if (!IsThereEmptyCell(game->board, N)) {
		if (game->board->erroneous == true) {
			printErroneousBoardError();
			return 0;
		}
		else {
			printf("Puzzle solved successfully!\n");
			UpdateGame(game, NULL , initMode);
			return 1;
		}
	}
	return 0;
}
/**
 * initialize board values and params to zero.
 */
Board* initialize(blocksize block) {
	int i, j, sizeOfRow;
	Cell** arrayBoard;
	Board* board;
	sizeOfRow = block.n * block.m;
	arrayBoard = malloc(sizeOfRow * sizeof(Cell*));
	if (!arrayBoard) {
		funcFailed("malloc");
	}
	for (i = 0;i < sizeOfRow;i++)
	{
		arrayBoard[i] = malloc(sizeof(Cell) * sizeOfRow);
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
	board = malloc(sizeof(Board));
	if (!board) {
		funcFailed("malloc");
	}
	board->erroneous = false;
	board->cells = arrayBoard;
	board->blocksize.m = block.m;
	board->blocksize.n = block.n;
	return board;
}

Game* initializeGame() {
	Game* game = (Game *)malloc(sizeof(Game));
	if (!game) {
		funcFailed("malloc");
	}
	game->board = NULL;
	game->userMoves=initializeDoublyLinkedList();
	game->curMove = NULL;
	game->mode = initMode;
	game->markErrors = true;
	return game;
}

void UpdateGame(Game* game, Board *userBoard, modes mode){
	game->mode = mode;
	game->board = userBoard;
}


/**
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
					if (game->board->cells[i][j].fixed == true) {
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

int getLegalGuess(Game* game,LPsol* lpSol, int row, int col, float threshold, int* legalValues) {
	int numOflegalValues, N, val, ix;
	index ind;
	numOflegalValues = 0;
	N = game->board->blocksize.m * game->board->blocksize.n;
	ind.col = col;
	ind.row = row;
	for (val = 1; val <= N; val++) {
		ix = game->board->cells[row][col].ixMap[val - 1] - 1;
		if (ix >= 0) {
			if (lpSol->solBoard[ix] >= threshold) {
				if (isValidOption(game, ind, val, false)) {
					legalValues[numOflegalValues++] = val;
				}
			}
		}
	}
	return numOflegalValues;
}

double* getScoresOfLegalValue(Game* game, LPsol* lpsol, int row, int col, int numOflegalValues, int* legalValues) {
	int i, val, ix;
	double* scores;
	scores = malloc(numOflegalValues * sizeof(double));
	if (!scores) {
		funcFailed("malloc");
	}
	for (i = 0; i < numOflegalValues; i++) {
		val = legalValues[i];
		if (val > 0) {
			ix = game->board->cells[row][col].ixMap[val - 1] - 1;
			scores[i] = lpsol->solBoard[ix];
		}
	}
	return scores;
}

int getRandIndex(int numOflegalValues, double* scores) {
	int i;
	double random;
	float sum, accumulative;
	accumulative = 0;
	sum = 0;
	if (numOflegalValues == 1) {
		return 0;
	}
	for (i = 0; i < numOflegalValues; i++) {
		sum += scores[i];
	}
	if (sum == 0) { /* all scores equal to zero then pick randomly */
		random = rand() % numOflegalValues;
		return random;
	}
	for (i = 0; i < numOflegalValues; i++) {/* make score's sum equal to 1 */
		scores[i] = scores[i] / sum;
	}
	random = (double)rand() / (float)(RAND_MAX)+1; /* random must be in range [0,1] */

	for (i = 0; i < numOflegalValues; i++) { 
		accumulative += scores[i]; 
		if (random < accumulative) {
			return i;
		}
	}
	return 0;
}

linkedList* createGenerateMoveList(Board* newBoard, Board* orignalBoard) {
	int i, j, row, col, newVal, prevVal;
	linkedList* move;
	move = initializeLinkedList();
	row = orignalBoard->blocksize.n;
	col = orignalBoard->blocksize.m;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			prevVal = orignalBoard->cells[i][j].value;
			newVal = newBoard->cells[i][j].value;
			if (prevVal != newVal) {
				insertLast(move, i, j, newVal, prevVal);
			}
		}
	}
	return move;
}

void printCommandSyntax(Command c, int maxVal) {
	if (c == solve) {
		printf("The correct syntax for solve command is: solve x.\nx should include a full or relative path to the file.\n");
	}
	if (c == edit) {
		printf("The correct syntax for edit command is: edit x.\nx should include a full or relative path to the file \
and this parameter is optional.\n If no parameter is supplied, the default board is an empty 9X9 board\n");
	}
	if (c == mark_errors) {
		printf("The correct syntax for mark_errors command is: mark_errors x.\n X should be either 0 or 1.\n");
	}
	if (c == print_board) {
		printf("The correct syntax for print_board command: print_board (with no extra parameters).\n");
	}
	if (c == set) {
		printf("The correct syntax for set command: set x y z (sets the value of cell<x,y> to z).\n");
		printf("x and y are integers between 1 to %d.\n", maxVal);
		printf("z is an integer between 0 to %d.\n", maxVal);
	}
	if (c == validate) {
		printf("The correct syntax for validate command:validate (with no extra parameters).\n");
	}
	if (c == guess) {
		printf("The correct syntax for guess command:guess x.\n X should be a float in range [0-1].\n");
	}
	if (c == generate) {
		printf("The correct syntax for generate command:generate x y.\n");
		printf("X and y are integers between 0 to %d.\n", maxVal);
	}
	if (c == undo) {
		printf("The correct syntax for undo command:undo (with no extra parameters).\n");
	}
	if (c == redo) {
		printf("The correct syntax for redo command:redo (with no extra parameters).\n");
	}
	if (c == save) {
		printf("The correct syntax for save command is: save x.\n X includes a full or relative path to the file.\n");
	}
	if (c == hint) {
		printf("The correct syntax for hint command is: hint x y\n");
		printf("X and y are integers between 1 to %d.\n", maxVal);
	}
	if (c == guess_hint) {
		printf("The correct syntax for guess_hint command is: guess_hint x y\n");
		printf("X and y are integers between 1 to %d.\n", maxVal);
	}
	if (c == num_solutions) {
		printf("The correct syntax for num_solutions command:num_solutions (with no extra parameters).\n");
	}
	if (c == autofill) {
		printf("The correct syntax for autofill command:autofill (with no extra parameters).\n");
	}
	if (c == reset) {
		printf("The correct syntax for reset command:reset (with no extra parameters).\n");
	}
	if (c == exitCommand) {
		printf("The correct syntax for exit command:exit (with no extra parameters).\n");
	}

}

int argsNum(char** move) {
	int cnt, i;
	cnt= 0;
	i = 0;
	while (move[i] != NULL) {
		cnt++;
		i++;
	}
	return cnt;
}

int checkParamsNum(int validNum, int paramsNum, Command c, int maxVal) {
	if (paramsNum == validNum) {
		return 1;
	}
	if (paramsNum < validNum) {
		printf("Error: Not enough parameters were entered.\n");
	}
	else if (paramsNum > validNum) {
		printf("Error: Extra parameters were entered.\n");
	}

	printCommandSyntax(c, maxVal);
	return 0;

}

int isFloat(char* num) {
	int i;
	i = 0;
	while (num[i] != '\0') {
		if (!(isNum(num[i]) || (num[i] == '.'))) {
			return 0;
		}
		i++;
	}
	if(num[0] == '0' && num[1] == '.'){
		return 1;
	}
	return 0;
}

int isNums(char* move) {
	int i;
	i = 0;
	while (move[i] != '\0') {
		if (isNum(move[i]) == 0) {
			return 0;
		}
		i++;
	}
	return 1;
}

int isNum(char c) {
	if (c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

int isInRange(int value, int max, int min) {
	if (value <= max && value >= min){
		return 1;
	}
	else{
		return 0;
	}
}

int validateSolve(char* move[]) {
	int paramsNum, checkparamsnum;
	paramsNum= argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, solve, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validateEdit(char** move) {
	int paramsNum;
	paramsNum = argsNum(move);

	if (paramsNum>2) {
		printf("Error: Extra parameters were entered.\n");
		printCommandSyntax(edit, 0);
		return 0;
	}
	return 1;
}

int validateMarkErrors(char* move[],Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
		printErrorMode("edit");
		}
		else{
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, mark_errors, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validatePrintBoard(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(1, paramsNum, print_board, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validateSet(char* move[],Game* game) {
	int paramsNum, checkparamsnum,n,m,maxValue;
	n = game->board->blocksize.n;
	m = game->board->blocksize.m;
	maxValue = n * m;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(4, paramsNum, set, maxValue);
	if (!checkparamsnum) {
		return 0;
	}

	if (!(isValidSetParams(move[1],move[2],move[3],game))) {
		return 0;
	}
return 1;
}

int isValidSetParams(char* x, char* y, char* z, Game* game) {
	int maxValue, m, n;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	maxValue = n * m;
	if (! (isNums(x)  && isInRange(atoi(x), maxValue, 1))) {
		printlegalRange("first", "integer", 1, maxValue);
		return 0;
	}
	if (!(isNums(y) && isInRange(atoi(y), maxValue, 1))) {
		printlegalRange("second", "integer", 1, maxValue);
		return 0;
	}
	if (!(isNums(z) && isInRange(atoi(z), maxValue, 0))) {
		printlegalRange("third", "integer", 0, maxValue);
		return 0;
	}
	return 1;
}

int validateValidate(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	if (game->board == NULL) {
		printBoardNotInit();
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(1, paramsNum, validate, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validateGuess(char* move[],Game* game) {
	int paramsNum, checkparamsnum, x;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, guess, 0);
	if (!checkparamsnum) {
		return 0;
	}
	if (!isFloat(move[1])) { 
		printlegalRange("first", "integer", 0, 1);
		return 0;
	}
	else {
		x = atof(move[1]);
		if (!(isInRange(x, 1, 0))) {
			printlegalRange("first", "integer", 0, 1);
			return 0;
		}
	}
	return 1;
}

int validateGenerate(char* move[], Game* game) {
	int paramsNum, checkparamsnum, n, m, N;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = m * n;
	if (game->mode != editMode) {
		if (game->mode == solveMode) {
			printErrorMode("solve");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(3, paramsNum, generate, N*N);
	if (!checkparamsnum) {
		return 0;
	}
	if (!(isValidTwoParams(move[1],move[2],0,N*N))) {
		return 0;
	}

	return 1;
}

int isValidTwoParams(char* x, char* y, int minValue, int maxValue) {
	if (!isNums(x) && isInRange(atoi(x), maxValue, minValue))
	{
		printlegalRange("first", "integer", minValue, maxValue);
		return 0;
	}
	if (!isNums(y) && isInRange(atoi(y), maxValue, minValue))
	{
		printlegalRange("second", "integer", minValue, maxValue);
		return 0;
	}
	return 1;
}

int validateUndoAndRedo(char* move[], Game* game, int isUndo) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	if (isUndo) {
		checkparamsnum = checkParamsNum(1, paramsNum, undo, 0);
	}
	else {
		checkparamsnum = checkParamsNum(1, paramsNum, redo, 0);
	}
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validateSave(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(2, paramsNum, save, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validateHintAndGuessHint(char* move[], Game* game, int isHint) {
	int paramsNum, checkparamsnum, n, m, maxValue;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	maxValue = m * n;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	if (isHint) {
		checkparamsnum = checkParamsNum(3, paramsNum, hint, maxValue);
	}
	else {
		checkparamsnum = checkParamsNum(3, paramsNum, guess_hint, maxValue);
	}
	if (!checkparamsnum) {
		return 0;
	}
	if (!isValidTwoParams(move[1], move[2], 1, maxValue)){
		return 0;
}
	return 1;
}

int validateAutofill(char* move[], Game* game) {
	int paramsNum, checkparamsnum;
	if (game->mode != solveMode) {
		if (game->mode == editMode) {
			printErrorMode("edit");
		}
		else {
			printErrorMode("init");
		}
		return 0;
	}
	paramsNum = argsNum(move);
	checkparamsnum = checkParamsNum(1, paramsNum, autofill, 0);
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

int validateNumSolAndExitAndReset(char* move[], Game* game, Command c) {
	int paramsNum, checkparamsnum;
	if (game->mode == initMode) {
		printErrorMode("init");
		return 0;
	}
	paramsNum = argsNum(move);
	if (c == num_solutions) {
		checkparamsnum = checkParamsNum(1, paramsNum, num_solutions, 0);
	}
	else if (c == exitCommand) {
		checkparamsnum = checkParamsNum(1, paramsNum, exitCommand, 0);
	}
	else if (c == reset) {
		checkparamsnum = checkParamsNum(1, paramsNum, reset, 0);
	}
	if (!checkparamsnum) {
		return 0;
	}
	return 1;
}

void markErroneous(Game* game) {
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
				check = isValidOption(game, ind, val , true);
				if (check == true) {
					game->board->cells[i][j].error = false;
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
bool isValidOption(Game* game, index ind, int value, bool mark) {
	bool checkBox, checkRowCol;
	index box;
	box = findBoxIndex(game, ind);
	checkBox = checkInBox(game, box, ind, value, mark);
	checkRowCol = checkInRowAndCol(game, ind, value, mark);
	if (checkBox && checkRowCol) {
		return true;
	}
	return false;
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
bool checkInBox(Game* game, index box, index ind, int value, bool mark) {
	int i, j, valBox, m, n;
	bool res;
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
				res = false;
				game->board->cells[box.row + i][box.col + j].error = mark;
			}
		}
	}
	return res;
}
/**
 * the function check if @param value is in the same row and column.
 *  if so returns false. else, returns true. if mark=true ,
 *  the function marks errornous cells in the same row and column.
 */
bool checkInRowAndCol(Game* game, index index, int value, bool mark) {
	int valRow, valCol, i, n, m;
	bool res;
	res = true;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	for (i = 0; i < n * m; i++) {
		if (i != index.row) {
			valCol = game->board->cells[i][index.col].value;
			if (valCol == value) {
				res = false;
				game->board->cells[i][index.col].error = mark;
			}
		}
		if (i != index.col) {
			valRow = game->board->cells[index.row][i].value;
			if (valRow == value) {
				res = false;
				game->board->cells[index.row][i].error = mark;
			}
		}
	}
	return res;
}

