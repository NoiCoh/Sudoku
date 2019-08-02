#include "Solver.h"

/**
 * the random recursive algorithm to solve a sudoku board.
 * the function checks if the board is solvable. if the board is solvable return solved,
 * else return unsolved.
 */
SudokuSolved RandBacktracking(Game* game){
    int val;
    index ind;
    if(!IsThereEmptyCell(game->board)){
        return solved;
    }else{
        ind = FindEmptyCell(game->board);
    }
    findOptionsCell(game,ind);
    val=getRandValue(game->board,ind);
    if (val!=0){
        game->board->cells[ind.row][ind.col].value=val;
        if(RandBacktracking(game)==solved){
            return solved;
        }else{
            return unsolved;
        }
    }else {
        if(goBackRand(game, ind)==solved){
            return solved;
        }else{
            return unsolved;
        }
    }
}
/**
 * an auxiliary function for the random backtracking algorithm.
 * if there is no valid options for a specific cell, the algorithm leaves the cell blank
 * and moves back (backtracks) to the previous cell.
 */
SudokuSolved goBackRand(Game* game, index ind){
    int row,col,val;
    row= ind.row;
    col= ind.col;
    while ( col+row !=0) {
        if (col == 0) {
            row = row - 1;
            col = 8;
        } else {
            col = col - 1;
        }
        ind.col = col;
        ind.row = row;
        val  = getRandValue(game->board, ind);
        if (game->board->cells[ind.row][ind.col].fixed == false && game->board->cells[ind.row][ind.col].userInput == false) {
            if (val != 0) { /**if val==0 there is no option for cell **/
                game->board->cells[ind.row][ind.col].value = val;
                RandBacktracking(game);
                return solved;
            }else {
                game->board->cells[ind.row][ind.col].value = 0;
            }
        }
    }
    return unsolved;
}
/**
 * the deterministic recursive algorithm to solve a Sudoku board.
 * the function checks if the board is solvable. if the board is solvable return solved,
 * else return unsolved.
 */
SudokuSolved deterministicBacktracking(Game* game){
    int val;
    index ind;
    if(!IsThereEmptyCell(game->board)) {
        return solved;
    }else{
        ind = FindEmptyCell(game->board);
    }for(val=1; val<=9;val++){
        if(isValidOption(game,ind,val,false)) {
            game->board->cells[ind.row][ind.col].value = val;
            if(deterministicBacktracking(game)==solved){
                return solved;
            }
            game->board->cells[ind.row][ind.col].value = 0;
        }
    }
    return unsolved;
}

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
 * the function checks if @param value is in the same row ,column and box of the cell in @param index.
 * if so returns false. else, returns true.
 * if mark=true we mark erroneous cells, if mark=false we unmark erroneous cells
 * (for example,when setting a cell to zero).
 */
bool isValidOption(Game* game,index ind,int value, bool mark ){
    index box;
    box= findBoxIndex(game, ind);
    if (checkInRowAndCol(game,ind,value,mark) && checkInBox(game,box,value, mark)){
        return true;
    }
    return false;
}
/**
 * the function finds the starting cell index of the the block that @param index belongs to.
 */
index findBoxIndex(Game* game,index ind){
    int m,n;
    index boxIndex;
    m=game->board->blocksize.m;
    n=game->board->blocksize.n;
    boxIndex.col= ind.col - ind.col%m;
    boxIndex.row= ind.row - ind.row%n;
    return boxIndex;
}
/**
 * the function checks if the @param value is in the block starting at @param index block. if mark=true , 
 * the function marks errornous cells in the same block.
 */
bool checkInBox(Game* game,index box,int value, bool mark){
    int i,j,valBox, m,n;
    bool res;
    res=true;
    m=game->board->blocksize.m;
    n=game->board->blocksize.n;
    for (i= 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            valBox= game->board->cells[box.row + i][ box.col + j].value;
            if ( valBox==value) {
                res = false;
                game->board->cells[box.row + i][ box.col + j].error=mark;
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
bool checkInRowAndCol(Game* game,index index, int value,bool mark){
    int valRow, valCol, i, n,m;
    bool res;
    res=true;
    m=game->board->blocksize.m;
    n=game->board->blocksize.n;
    for(i=0; i<n*m; i++){
        valCol=game->board->cells[i][index.col].value;
        valRow=game->board->cells[index.row][i].value;
        if(valRow==value){
            res= false;
            game->board->cells[index.row][i].error=mark;
        }
        if(valCol==value){
            res= false;
            game->board->cells[i][index.col].error=mark;
        }
    }
    return res;
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
 * the function checks if there is an empty cell in the board.
 * if so, returns true. else, returns false.
 */
bool IsThereEmptyCell(Board* board){
    int row;
    int col;
    for(row=0; row<9; row++) {
        for (col = 0; col < 9; col++) {
            if (board->cells[row][col].value==0) {
                return true;
            }
        }
    }
    return false;
}
/**
 * the function returns the index of the first empty cell.
 */
index FindEmptyCell(Board* board){
    int row,col;
    index i;
    i.col=0;
    i.row=0;
    for(row=0; row<9; row++) {
        for (col = 0; col < 9; col++) {
            if (board->cells[row][col].value==0) {
                i.col = col;
                i.row = row;
                return i;
            }
        }
    }
    return i;
}

int exhaustiveBacktracking() {
	return 0;
}

