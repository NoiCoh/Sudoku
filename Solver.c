#include "Solver.h"

/**
 * the random recursive algorithm to solve a sudoku board.
 * check if the board is solvable and return solved, else return unsolved.
 */
SudokuSolved RandBacktracking(Board* board){
    int val;
    index ind;
    if(!IsThereEmptyCell(board)){
        return solved;
    }else{
        ind = FindEmptyCell(board);
    }
    findOptionsCell(board,ind);
    val=getRandValue(board,ind);
    if (val!=0){
        board->cells[ind.row][ind.col].value=val;
        if(RandBacktracking(board)==solved){
            return solved;
        }else{
            return unsolved;
        }
    }else {
        if(goBackRand(board, ind)==solved){
            return solved;
        }else{
            return unsolved;
        }
    }
}
/**
 * Auxiliary function for random backtracking algorithm.
 * if there is no valid options for a specific cell, then the algorithm leaves the cell blank
 * and moves back (backtracks) to the previous cell.
 */
SudokuSolved goBackRand(Board* board, index ind){
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
        val  = getRandValue(board, ind);
        if (board->cells[ind.row][ind.col].fixed == false && board->cells[ind.row][ind.col].userInput == false) {
            if (val != 0) { /**if val==0 there is no option for cell **/
                board->cells[ind.row][ind.col].value = val;
                RandBacktracking(board);
                return solved;
            }else {
                board->cells[ind.row][ind.col].value = 0;
            }
        }
    }
    return unsolved;
}
/**
 * the deterministic recursive algorithm to solve a Sudoku board.
 * check if the board is solvable and return solved, else return unsolved.
 */
SudokuSolved deterministicBacktracking(Board* board){
    int val;
    index ind;
    if(!IsThereEmptyCell(board)) {
        return solved;
    }else{
        ind = FindEmptyCell(board);
    }for(val=1; val<=9;val++){
        if(isValidOption(board,ind,val)) {
            board->cells[ind.row][ind.col].value = val;
            if(deterministicBacktracking(board)==solved){
                return solved;
            }
            board->cells[ind.row][ind.col].value = 0;
        }
    }
    return unsolved;
}

/**
 * choose a random value from option array of a specific cell.
 * when only a single legal value remains, the function returns that value.
 * if there are no options remains return 0.
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
 * create an array of valid options for a specific cell.
 */
void findOptionsCell(Board* board,index ind){
    int val,count;
    count=0;
    for(val=1;val<=9;val++){
        if(isValidOption(board,ind,val)){
            board->cells[ind.row][ind.col].options[count]=val;
            count++;
        }
    }
    board->cells[ind.row][ind.col].optionsSize=count;
}
/**
 * check if @param value is in the same row ,column and box of the cell in @param index.
 * if so return false. else, return true.
 */
bool isValidOption(Board* board,index ind,int value){
    index box;
    box= findBoxIndex(ind);
    if (checkInRowAndCol(board,ind,value) && checkInBox(board,box,value)){
        return true;
    }
    return false;
}
/**
 * find the starting cell index of the the box that @param index is belong to.
 */
index findBoxIndex(index ind){
    index boxIndex;
    boxIndex.col= ind.col - ind.col%3;
    boxIndex.row= ind.row - ind.row%3;
    return boxIndex;
}
/**
 * check if the @param value is in the box starting at @param index box.
 */
bool checkInBox(Board* board,index box,int value){
    int i,j,valBox;
    for (i= 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            valBox= board->cells[box.row + i][ box.col + j].value;
            if ( valBox==value){
                return false;
            }
        }
    }
    return true;
}
/**
 * check if @param value is in the same row and column.
 *  if so return false. else, return true.
 */
bool checkInRowAndCol(Board* board,index index, int value){
    int valRow, valCol, i;
    for(i=0; i<9; i++){
        valCol=board->cells[i][index.col].value;
        valRow=board->cells[index.row][i].value;
        if(valRow==value){
            return false;
        }
        if(valCol==value){
            return false;
        }
    }
    return true;
}
/**
 * check size of option array for a specific cell.
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
 * remove zero elements from option array.
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
 * check if there is an empty cell in the board.
 * if so, return true. else, return false.
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
 * return the index of the first empty cell.
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

