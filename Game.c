#include "Game.h"


/**
 * initialize board values and params to zero.
 */
Board* initialize(){
    int i,j;
    Cell **arrayBoard;
    Board* board;
    arrayBoard = malloc(SIZE_OF_BORAD*sizeof(Cell *));
    if(!arrayBoard){
        funcFailed("malloc");
    }
    for(i=0;i<SIZE_OF_BORAD;i++)
    {
        arrayBoard[i] = malloc(sizeof(Cell)*SIZE_OF_BORAD);
        if(!arrayBoard[i]){
            funcFailed("malloc");
        }
    }
    for(i=0;i<SIZE_OF_ROW_COL;i++){
        for(j=0;j<SIZE_OF_ROW_COL;j++){
            arrayBoard[i][j].value = 0;
            arrayBoard[i][j].fixed = false;
            arrayBoard[i][j].userInput = false;
            arrayBoard[i][j].optionsSize = 0;
            arrayBoard[i][j].options = calloc(SIZE_OF_ROW_COL, sizeof(int));
            if(!arrayBoard[i][j].options){
                funcFailed("malloc");
            }
        }
    }
    board = malloc(sizeof(arrayBoard));
    if(!board){
        funcFailed("malloc");
    }
    board->cells = arrayBoard;
    board->solved=false;
    return board;
}

/**
 * the function returns user board with random fixed cells from the solution board and empty cells.
 * the number of fixed cell is according to the user's input (@param hint).
 */
Board* makeUserBoard(Board* solvedBoard,int hint){
    int count, x, y;
    Board* userBoard;
    count =0;
    userBoard = initialize();
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
void setCommand(Board* userBoard, char* x, char* y, char* z){
    int row, col, val;
    index ind;
    /**convert string to int**/
    col= atoi(x)-1;
    row= atoi(y)-1;
    val= atoi(z);
    ind.col=col;
    ind.row=row;
    if(userBoard->cells[row][col].fixed==true){
        printf("Error: cell is fixed\n");
    }else if(val==0){
        userBoard->cells[row][col].value=val;
        userBoard->cells[row][col].userInput=false;
        printBoard(userBoard);
    }else if(!isValidSet(userBoard,ind,val)){
        printf("Error: value is invalid\n");
    }else{
        userBoard->cells[row][col].value=val;
        userBoard->cells[row][col].userInput=true;
        printBoard(userBoard);
  	if(!IsThereEmptyCell(userBoard)){
            printf("Puzzle solved successfully\n");
            userBoard->solved=true;
        }
    }
}
/**
 * check the user value during setCommand.
 * check if the value is invalid (if the value is already in the same box, row or column).
 * if the value is valid returns true, else returns false.
 */
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
}
/**
 *
 *gives a hint to the user by showing a possible legal value for a single cell <x,y> (x is the column and y is the row).
 *hint is given according to the solved board, and it is given even if it is incorrect for the current state of the board.
 *
 */
void hintCommand(Board* solBoard,char* x, char* y){
    int row, col, z;
    col= atoi(x) -1;
    row= atoi(y) -1;
    z= solBoard->cells[row][col].value;
    printf("Hint: set cell to %d\n",z);
}
/**
 * validates that thr current state of the board is solvable.
 * solved the generated board with deterministic backtracking.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
void validate(Board* userBoard,Board* solBoard){
    SudokuSolved res;
    Board* checkBoard;
    checkBoard= initialize();
    makeCopyBoard(userBoard,checkBoard);
    res = deterministicBacktracking(checkBoard);
    if(res == solved){
        printf("Validation passed: board is solvable\n");
makeCopyBoard(checkBoard,solBoard);
    }else if(res == unsolved){
        printf("Validation failed: board is unsolvable\n");
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

void freeBoard(Board *currentBoard)
{
	int k,l;
	if(currentBoard){
		for(k=0;k<SIZE_OF_ROW_COL;k++)
		{
			for(l=0;l<SIZE_OF_ROW_COL;l++)
			{
				if(currentBoard->cells[k][l].options){
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

