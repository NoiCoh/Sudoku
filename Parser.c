/*
 * Parser:
 * This module handles receiving input from the user and parse it.
 */
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * the function reads the user's command.
 */
void readUser(Game* game) {
	char ch, input[256], * move[256], delimiter[] = " \t\r\n";
	int i;
	i = 0;
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
 * The function checks the user's command. if it is a valid command, execute it. 
 * if the user enters a command that doesn't match any of the commands defined,  the program prints an error.
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
				markErrorsCommand(move[1], game);
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
				setCommand(game, row, col, val,true);
				printBoard(game);
			}
			
		}
		else if ((strcmp(move[0], "validate") == 0)) {
			if (validateValidate(move, game)) {
				validateCommand(game,0);
				printBoard(game);
			}
		}
		else if ((strcmp(move[0], "guess") == 0)) {
			if (validateGuess(move, game)) {
				guessCommand(game, atof(move[1]));
			}
			printBoard(game);
		}
		else if ((strcmp(move[0], "generate") == 0)) {
			if (validateGenerate(move, game)) {
				generateCommand(game, atoi(move[1]), atoi(move[2]));
			}
			printBoard(game);
		}
		else if (strcmp(move[0], "undo") == 0) {
			if (validateUndoAndRedo(move, game, 1)) {
				undoCommand(game,true);
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
				hintCommand(game, move[1], move[2]);
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
			if (validateNumSolAndExitAndReset(move,game,num_solutions,0)){
				numSolution(game);
			}
				printBoard(game);
		}
		else if ((strcmp(move[0], "reset") == 0)) {
			if (validateNumSolAndExitAndReset(move, game, reset,0)) {
				if (game->curMove == game->userMoves->head) {
					printf("The Board is already in its original loaded state\n");
				}
				else {
					resetCommand(game);
				}
			}

			printBoard(game);
		}
		else if (strcmp(move[0], "exit") == 0) {
			if (validateNumSolAndExitAndReset(move, game, exitCommand,1)) {
				exiting(game);
			}
		}
		else {
		printf("Error: invalid command\n");
		}
	}
}






