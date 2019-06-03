#include "MainAux.h"

/**
 * Prints the current state of @param board
 */
void printBoard(Board* board){
    int i,j,val;
    for(i=0; i<9;i++){
        if (i%3==0){
            printf("----------------------------------\n");
        }
        for(j=0;j<9;j++){
            if (j%3==0){
                printf("| ");
            }
            val = board->cells[i][j].value;
            if (val==0){
                printf("   ");
            }else{
                if(board->cells[i][j].fixed==true){
                    printf(".");
                }else{
                    printf(" ");
                }
                printf("%d ", val);
            }
        }
        printf("|\n");
    }
    printf("----------------------------------\n");
}
/**
 * Make a deep copy of @param board to @param copyBorad.
 */
void makeCopyBoard(Board* board, Board* copyBorad) {

    int i, j;
    for (i = 0; i < SIZE_OF_ROW_COL; i++) {
        for (j = 0; j < SIZE_OF_ROW_COL; j++) {
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
 *  If we reach EOF, the program prints "Exiting…" and terminates.
 */
void checkEOF(){
    if(feof(stdin)){
        printf("Exiting...\n");
        exit(0);
    }
}


