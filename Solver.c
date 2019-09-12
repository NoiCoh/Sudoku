#include "Solver.h"
#include <time.h>


/**
 * the function chooses a random value from the options array of a specific cell.
 * when only a single legal value remains, the function returns that value.
 * if there are no options remains -returns 0.
 */
int getRandValue(Board* board,index ind){
    int sizeOfOptions, val, randNum;
    int* options;
    options= board->cells[ind.row][ind.col].options;
    checkSizeArray(board,ind);
    sizeOfOptions= board->cells[ind.row][ind.col].optionsSize;
    if(sizeOfOptions==0){
        return 0;
    }
    if(sizeOfOptions==1){
        val = options[0];
        options[0]=0;
    }else{
        randNum = rand()%(sizeOfOptions);
        val = options[randNum];
        options[randNum]=0;
        removeZeroFromOptions(board,ind);
    }
    return val;
}
/**
 * the function creates an array of valid options for a specific cell.
 */
void findOptionsCell(Game* game,index ind){
    int val,count,m,n;
    count=0;
    m=game->board->blocksize.m;
    n=game->board->blocksize.n;
    for(val=1;val<=n*m;val++){
        if(isValidOption(game,ind,val,false)){
            game->board->cells[ind.row][ind.col].options[count]=val;
            count++;
        }
    }
    game->board->cells[ind.row][ind.col].optionsSize=count;
}

/**
 * the function checks the size of the options array for a specific cell.
 */
void checkSizeArray(Board* board,index ind){
    int i,count;
    count=0;
    for(i=0;i<9;i++){
        if (board->cells[ind.row][ind.col].options[i]!=0){
            count++;
        }else{
            break;
        }
    }
    board->cells[ind.row][ind.col].optionsSize=count;
}
/**
 * the function removes zero elements from option array.
 */
void removeZeroFromOptions(Board* board, index ind) {
    int i, j;
    int* old;
    old = board->cells[ind.row][ind.col].options;
    j=0;
    for (i = 0; i < 9; i++) {
        if (old[i] != 0) {
            board->cells[ind.row][ind.col].options[j] = old[i];
            j++;
        }
    }
    for(i=j;i<9;i++){
        board->cells[ind.row][ind.col].options[i]=0;
    }
}


/**
 * the function returns the index of the first empty cell.
 */
index FindEmptyCell(Board* board,int N){
    int row,col;
    index i;
    i.col=0;
    i.row=0;
    for(row=0; row<N; row++) {
        for (col = 0; col < N; col++) {
            if (board->cells[row][col].value==0) {
                i.col = col;
                i.row = row;
                return i;
            }
        }
    }
    return i;
}

/**
 * the deterministic recursive algorithm to solve a Sudoku board.
 * the function checks if the board is solvable. if the board is solvable return solved,
 * else return unsolved.
 */
SudokuSolved deterministicBacktracking(Game* game, int N) {
	int val;
	index ind;
	if (!IsThereEmptyCell(game->board, N)) {
		return solved;
	}
	else {
		ind = FindEmptyCell(game->board, N);
	}for (val = 1; val <= N;val++) {
		if (isValidOption(game, ind, val, false)) {
			game->board->cells[ind.row][ind.col].value = val;
			if (deterministicBacktracking(game, N) == solved) {
				return solved;
			}
			game->board->cells[ind.row][ind.col].value = 0;
		}
	}
	return unsolved;
}
/**
 * the deterministic algorithm to solve a Sudoku board with a stack implementation
 */
SudokuSolved deterministicBacktrackingWithStack(Game* game, int N) {
	int val;
	index ix;
	bool tried_all;
	Stack stack;
	cellMem* prevCellMem;
	val = 1;
	initStack(&stack);
	tried_all = false;
	while(IsThereEmptyCell(game->board, N)) {
		if (!tried_all) val = 1;
		ix = FindEmptyCell(game->board, N);
		tried_all = true;
		for (; val <= N; val++) {
			if (isValidOption(game, ix, val, false)) {
				game->board->cells[ix.row][ix.col].value = val;
				push(ix,val,&stack);
				tried_all = false;
				break;
			}
		}
		if (tried_all) {
			if (stackIsEmpty(&stack)) {
				return unsolved;
			}
			prevCellMem = pop(&stack); /* previous index we handle in the stack */
			game->board->cells[prevCellMem->ix.row][prevCellMem->ix.col].value = 0; 
			val = prevCellMem->val;
			val = val + 1;
		}
	}
	return solved;
}

/**
 * count the number of solutions of the current board using backtracking algorithm.
 */
int exhaustiveBacktracking(Game* game, int N) {
	int counter, k, i, j, val;
	index ind;
	bool doneGoBack;
	counter = 0;
	while (1) {
		if (deterministicBacktrackingWithStack(game, N) == solved) {
			counter++;
			i = N - 1;
			j = N - 1;
		}
		else {
			ind = FindEmptyCell(game->board, N);
			j = ind.col;
			i = ind.row;
			pre(&i, &j, N);
			while (game->board->cells[i][j].fixed == true || game->board->cells[i][j].userInput == true) {
				if (i == 0 && j == 0) {
					return counter;
				}
				pre(&i, &j, N);
			}
		}
		doneGoBack = false;
		while (doneGoBack == false) {
			val = game->board->cells[i][j].value;
			for (k = val + 1; k <= N; k++) {
				ind.col = j;
				ind.row = i;
				if (isValidOption(game, ind, k, false)) {
					game->board->cells[i][j].value = k;
					doneGoBack = true;
					break;
				}
			}
			if (doneGoBack == false) {
				game->board->cells[i][j].value = 0;
				if (i == 0 && j == 0) {
					return counter;
				}
				pre(&i, &j, N);
				while (game->board->cells[i][j].fixed == true || game->board->cells[i][j].userInput == true) {
					if (i == 0 && j == 0) {
						return counter;
					}
					pre(&i, &j, N);
				}
			}
		}
	}
}
/**
* an auxiliary function that finds the previous cell index
*/
void pre(int* i, int* j,int N) {
	if (*j == 0) {
		*i = *i - 1;
		*j = N-1;
	}
	else {
		*j = *j - 1;
	}
}
