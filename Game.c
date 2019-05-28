#include "Game.h"
#define SIZE_OF_ROW_COL 9
#define SIZE_OF_BORAD 81

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
    count =0;
    Board* userBoard= initialize();
    while(count<hint){
        x= rand()%9;
        y= rand()%9;
        if(userBoard->cells[x][y].fixed==false){
            userBoard->cells[x][y].value=solvedBoard->cells[x][y].value;
            userBoard->cells[x][y].fixed=true;
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
    row= atoi(x)-1;
    col= atoi(y)-1;
    val= atoi(z);
    ind.col=col;
    ind.row=row;
    if(userBoard->cells[row][col].fixed==true){
        printf("Error: cell is fixed\n");
    }else if(!isValidSet(userBoard,ind,val)){
        printf("Error: value is invalid\n");
    }else if(val==0){
        userBoard->cells[row][col].value=val;
        userBoard->cells[row][col].userInput=false;
        printBoard(userBoard);
    }
    else{
        userBoard->cells[row][col].value=val;
        if(!IsThereEmptyCell(userBoard)){
            printf("Puzzle solved successfully\n");
            userBoard->solved=true;
        }
        userBoard->cells[row][col].userInput=true;
        printBoard(userBoard);
    }
}
/**
 * check the user value during setCommand.
 * check if the value is invalid (if the value is already in the same box, row or column).
 * if the value is valid returns true, else returns false.
 */
bool isValidSet(Board* userBoard,index ind, int val){
    int i;
    findOptionsCell(userBoard,ind);
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
    row= atoi(x) -1;
    col= atoi(y) -1;
    z= solBoard->cells[row][col].value;
    printf("Hint: set cell to %d\n",z);
}
/**
 * validates that thr current state of the board is solvable.
 * solved the generated board with deterministic backtracking.
 * if no solution is found prints: "Validation failed: board is unsolvable"
 * else, prints "Validation passed: board is solvable"
 */
void validate(Board* userBoard){
    SudokuSolved res;
    Board* checkBoard;
    checkBoard= initialize();
    makeCopyBoard(userBoard,checkBoard);
    res = deterministicBacktracking(checkBoard);
    if(res == solved){
        printf("Validation passed: board is solvable\n");
    }else if(res == unsolved){
        printf("Validation failed: board is unsolvable\n");
    }
}
/**
 * restart the game by starting over with the initialization procedure.
 */
void restart(){
    int num;
    Board* SolutionBoard;
    Board* userBoard;
    SolutionBoard = initialize();
    num = getUserNum();
    srand((unsigned int) num*time(0));
    RandBacktracking(SolutionBoard);
    printBoard(SolutionBoard);
    userBoard = makeUserBoard(SolutionBoard,num);
    printBoard(userBoard);
    readUser(userBoard,SolutionBoard);
}
/**
 * the function prints "Exiting..." , frees all memory resources and exits.
 */
void exiting(Board* userBoard,Board* board){
    printf("Exiting...\n");
    free(userBoard);
    free(board);
    exit(0);
}

