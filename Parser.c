#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>



/**
 * The game starts by asking the user to enter the number of cells to fill.
 * The user then enters a number between [0-80]. This is the number of "fixed" cells, cells with
 * values that never change throughout the game.
 */
int getNumCells(blocksize block) {
	int num, ok, m, n, N;
	num = 0;
	n = block.n;
	m = block.m;
	N = n * m;
	printf("Please enter the number of cells to fill [0-%d]:\n", (N * N) - 1);
	ok = scanf("%d", &num);
	checkEOF();
	num = checkVaild(num, N, block);
	if (ok <= 0) {
		printf("Error: not a number\n");
		printf("Exiting...\n");
		return 0;
	}
	return num;
}

/**
 * If the user enters an invalid number, the program prints an error message: "Error: invalid
 * number of cells to fill (should be between 0 and 80)", and lets the user try again.
 */
int checkVaild(int num, int N, blocksize block) {
	if (num > N || num < 0) {
		printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", N);
		return getNumCells(block);
	}
	return num;
}
/**
 * the function wait for the user command.
 */
void readUser(Game* game) {
	char ch, input[256], * move[256], delimiter[] = " \t\r\n";
	int i;
	i = 0;
	//fflush(stdin);
	printf("Enter your command:\n");
	while ((ch = fgetc(stdin)) != '\n') {
		if (i > 256) {
			printf("Error: Invalid command, too many characters in a single line\n");
			return;
		}
		if (ch == EOF) {
			checkEOF();
		}
		input[i] = ch;
		i++;
	}
	input[i] = '\0';
	i = 0;
	move[i] = strtok(input, delimiter);
	while (move[i] != NULL) {
		i++;
		move[i] = strtok(NULL, delimiter);
	}
	checkString(game, move);
	fflush(stdin);
}


/**
 * The function checks the user's command and execute one of the following commands: set, hint, validate, restart and exit.
 * if the user enters a command that doesn't match any of the commands defined the program prints "Error: invalid command".
 */
void checkString(Game* game, char* move[]) {
	int row, col, val;
	if (move[0] != NULL) {
		if (strcmp(move[0], "solve") == 0) {
			if (validateSolve(move)) {
				solveCommand(move[1], game);
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "edit") == 0) {
			if (validateEdit(move)) {
				editCommand(move[1], game);
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "mark_errors") == 0) {
			if (validateMarkErrors(move, game)) {
				markErrorsCommand(move[1], game, 1);
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "print_board") == 0)) {
			if (validatePrintBoard(move, game)) {
				printCommand(game);
			}
		}
		else if ((strcmp(move[0], "set") == 0)) {
			if (validateSet(move, game)) {
				/**convert string to int**/
				row = atoi(move[1]) - 1;
				col = atoi(move[2]) - 1;
				val = atoi(move[3]);
				setCommand(game, row, col, val);
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "validate") == 0)) {
			if (validateValidate(move, game)) {
				validateCommand(game);
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "guess") == 0)) {
			if (validateGuess(move, game)) {
				guessCommand(game, atof(move[1]));
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "generate") == 0)) {
			if (validateGenerate(move, game)) {
				generateCammand(game, atoi(move[1]), atoi(move[2]));
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "undo") == 0) {
			if (validateUndoAndRedo(move, game, 1)) {
				undoCommand(game, false);
			}
			printBoard(game);
		}

		else if (strcmp(move[0], "redo") == 0) {
			if (validateUndoAndRedo(move, game, 0)) {
				redoCommand(game);
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "save") == 0) {
			if (validateSave(move, game)) {
				saveGame(game, move[1]);
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "hint") == 0)) {
			if (validateHintAndGuessHint(move, game, 1)) {
				hintCommand(game, atoi(move[1]), atoi(move[2]));
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "guess_hint") == 0)) {
			if (validateHintAndGuessHint(move, game, 0)) {
				guessHintCommand(game, move[1], move[2]);
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "autofill") == 0) {
			if (validateAutofill(move,game)){
				autofillCommand(game);
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "num_solutions") == 0) {
			if (validateNumSolAndExitAndReset(move,game,num_solutions)){
				numSolution(game);
			}
				printBoard(game);
		}
		else if ((strcmp(move[0], "reset") == 0)) {
			if (validateNumSolAndExitAndReset(move, game, reset)) {
				resetCommand(game);
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "exit") == 0) {
		if (validateNumSolAndExitAndReset(move, game, exitCommand)) {
			exiting(game);
		}
		printBoard(game);
		}
	}
	else {
		printf("Error: invalid command\n");
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
	while (1) {
		readUser(game);
	}
}






