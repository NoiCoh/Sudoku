#include "Parser.h"

/**
 * The game starts by asking the user to enter the number of cells to fill.
 * The user then enters a number between [0-80]. This is the number of "fixed" cells, cells with
 * values that never change throughout the game.
 */
int getNumCells(){
    int num, ok;
    num=0;
    printf("Please enter the number of cells to fil [0-80]:\n");
    ok =scanf("%d",&num);
    checkEOF();
    checkVaild(num);
    if(ok <= 0 ){
        funcFailed("scanf");
    }
    return num;
}

/**
 * If the user enters an invalid number, the program prints an error message: "Error: invalid
 * number of cells to fill (should be between 0 and 80)", and lets the user try again.
 */
void checkVaild(int num){
    if(num>80 || num<0){
        printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
        getNumCells();
    }
}
/**
 * the function wait for the user command.
 */
void readUser(Board* userBoard,Board* solBoard){
    char input[1024], *move[1024], delimiter[]=" \t\r\n";
    int i;
    i=0;
    fflush(stdin);
    while(fgets(input,1024,stdin)) {
        move[i] = strtok(input, delimiter);
        while (move[i] != NULL) {
            i++;
            move[i] = strtok(NULL, delimiter);
        }
        checkString(userBoard, solBoard, move);
        fflush(stdin);
        i=0;
    }
}
/**
 * The function checks the user's command and execute one of the following commands: set, hint, validate, restart and exit.
 * if the user enters a command that doesn't match any of the commands defined the program prints "Error: invalid command".
 */
void checkString(Board* userBoard,Board* solBoard, char* move[]){
    if(move[0]!=NULL) {
        if ((strcmp(move[0], "set") == 0) && move[1] != NULL && move[2] != NULL && move[3] != NULL &&
            userBoard->solved == false) {
            setCommand(userBoard, move[1], move[2], move[3]);
        } else if ((strcmp(move[0], "hint") == 0) && move[1] != NULL && move[2] != NULL && userBoard->solved == false) {
            hintCommand(solBoard, move[1], move[2]);
        } else if ((strcmp(move[0], "validate") == 0) && userBoard->solved == false) {
            validate(userBoard);
        } else if ((strcmp(move[0], "restart") == 0)) {
            restart();
        } else if (strcmp(move[0], "exit") == 0) {
            exiting(userBoard, solBoard);
        } else {
            printf("Error: invalid command\n");
        }
    }
}
