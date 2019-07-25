#include "MainAux.h"

/**
 * Prints the current state of @param board
 */
void printBoard(Board* board) {
    int i, j, val, size, m, n;
    n = board->blocksize.n;
    m = board->blocksize.m;
    size = n * m;
    for (i = 0; i < size; i++) {
        if (i % n == 0) {
            printNTimes(4*size+m+1);
        }
        for (j = 0; j < size; j++) {
            if (j % m == 0) {
                printf("|");
            }
            val = board->cells[i][j].value;
            if (val == 0) {
                printf("    ");
            } else {
                printf(" %2d", val);
                if (board->cells[i][j].fixed == true) {
                    printf(".");
                } else if (board->cells[i][j].error == true) {
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

void printNTimes(int n){
    int i;
    for(i=0;i<n;i++){
        putchar('-');
    }
    putchar('\n');
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


