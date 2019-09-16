#include <stdio.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"


/**
 * Authors: Inbal Avivi and Noi Cohen 
 */
int main(int argc, char *argv[]) {
if (argc>1){
srand(atoi(argv[1]));
}else{
srand(0);
}
    restart();
    return (argc);
}