#include "MainAux.h"

void printErrorMode() {
	printf("Error: The command is not available ");
}

void printWelcome() {
	printf("Welcome to Sudoku Game!/n ");
}
/**
 * Prints the current state of @param board
 */
void printBoard(Board* board,bool markErrors) {
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
                } else if ((board->cells[i][j].error == true)&&(markErrors==true)) {
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


