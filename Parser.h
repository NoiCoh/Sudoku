#ifndef PARSER_H
#define PARSER_H
#include "Game.h"
#include <string.h>
#include "ParserAux.h"

/**
 * the function reads the user's command.
 */
void readUser(Game * game);

/**
 * The function checks the user's command. if it is a valid command, execute it.
 * if the user enters a command that doesn't match any of the commands defined,  the program prints an error.
 */
void checkString(Game * game,char* move[]);


#endif
