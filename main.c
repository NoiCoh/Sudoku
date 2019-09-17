/*
 * main:
 * This module starts the game.
 */

#include <stdio.h>
#include <time.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"

/*
 * Authors: Inbal Avivi and Noi Cohen 
 * Main function that starts the game and initilizes the seed for the functions that use random.
 */
int main() {
	srand(time(0));
    restart();
    return 1;
}