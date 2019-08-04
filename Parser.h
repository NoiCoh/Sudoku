#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "Game.h"



/**
 * The game starts by asking the user to enter the number of cells to fill.
 * The user then enters a number between [0-80]. This is the number of "fixed" cells, cells with
 * values that never change throughout the game.
 */
int getNumCells(blocksize block);

/**
 * If the user enters an invalid number, the program prints an error message: "Error: invalid
 * number of cells to fill (should be between 0 and 80)", and lets the user try again.
 */
int checkVaild(int num,int N, blocksize block);

/**
 * the function wait for the user command.
 */
void readUser(Game * game);

/**
 * The function checks the user's command and execute one of the following commands: set, hint, validate, restart and exit.
 * if the user enters a command that doesn't match any of the commands defined the program prints "Error: invalid command".
 */
void checkString(Game * game,char* move[]);

/**
 * restart the game by starting over with the initialization procedure.
 */
void restart();

bool isValidSet(char* x, char* y, char* z, Game* game);
bool isValidInput(char* x, char* y, int minValue, int maxValue);
bool isInRange(int value, int max, int min);
bool isfloatInRange(char* num);


#endif
