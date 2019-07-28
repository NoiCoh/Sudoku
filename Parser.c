#include "Parser.h"



blocksize getBoardSize(){
    int n,m, ok;
    n=0;
    m=0;
    printf("Please enter the block's size:\n");
    ok =scanf("%d %d",&n,&m);
    checkEOF();
    if(ok <2 ){
        printf("Error: not a number\n");
        printf("Exiting...\n");
        exit(0);
    }
    blocksize block;
    block.m=m;
    block.n=n;
    return block;
}


/**
 * The game starts by asking the user to enter the number of cells to fill.
 * The user then enters a number between [0-80]. This is the number of "fixed" cells, cells with
 * values that never change throughout the game.
 */
int getNumCells(blocksize block){
    int num, ok,m,n,N;
    num=0;
	n = block.n;
	m = block.m;
	N = n * m;
    printf("Please enter the number of cells to fill [0-%d]:\n",(N*N)-1);
    ok =scanf("%d",&num);
    checkEOF();
    num=checkVaild(num,N,block);
    if(ok <= 0 ){
        printf("Error: not a number\n");
		printf("Exiting...\n");
		exit(0);
    }
    return num;
}

/**
 * If the user enters an invalid number, the program prints an error message: "Error: invalid
 * number of cells to fill (should be between 0 and 80)", and lets the user try again.
 */
int checkVaild(int num,int N, blocksize block){
    if(num>N || num<0){
        printf("Error: invalid number of cells to fill (should be between 0 and %d)\n",N);
         return getNumCells(block);
    }
return num;
}
/**
 * the function wait for the user command.
 */
void readUser(Game * game){
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
        checkString(game, move);
        fflush(stdin);
        i=0;
checkEOF();
    }
checkEOF();

}

/**
 * The function checks the user's command and execute one of the following commands: set, hint, validate, restart and exit.
 * if the user enters a command that doesn't match any of the commands defined the program prints "Error: invalid command".
 */
void checkString(Game * game,char* move[]){
    if(move[0]!=NULL) {
		if ((strcmp(move[0], "solve") == 0) && move[1] != NULL && game->board->solved == false) {
			solveCommand(game->board, move[1]);
		}else if ((strcmp(move[0], "edit") == 0) && game->board->solved == false) {
				editCommand(game->board, move[1]);
		}else if ((strcmp(move[0], "mark_errors") == 0) && move[1] != NULL && game->board->solved == false) {
			markErrorsCommand(game->board, move[1]);
		}else if ((strcmp(move[0], "print_board") == 0)  ) {
			printCommand(game->board);
		}else if ((strcmp(move[0], "set") == 0) && move[1] != NULL && move[2] != NULL && move[3] != NULL &&
            game->board->solved == false) {
            setCommand(game->board, move[1], move[2], move[3]);
        } else if ((strcmp(move[0], "hint") == 0) && move[1] != NULL && move[2] != NULL && game->board->solved == false) {
            hintCommand(game->solBoard, move[1], move[2]);
        } else if ((strcmp(move[0], "validate") == 0) && game->board->solved == false) {
            validateCommand(game->board,game->solBoard, game->board->blocksize);
        } else if ((strcmp(move[0], "restart") == 0)) {
		            restart();
        } else if (strcmp(move[0], "exit") == 0) {
			
            exiting(game->board, game->solBoard);
        } else {
            printf("Error: invalid command\n");
        }
    }
}
/**
 * restart the game by starting over with the initialization procedure.
 */
void restart(){
	Game *game;
	printWelcome();
	game=initializeGame();
    /*int num;
    Board* SolutionBoard;
    Board* userBoard;

    blocksize block = getBoardSize();
    SolutionBoard = initialize(block);
    num = getNumCells(block);
    RandBacktracking(SolutionBoard);
    userBoard = makeUserBoard(SolutionBoard,num,block);
    printBoard(userBoard);
	*/
    readUser(game);
}
