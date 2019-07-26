#include <stdio.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#include "ILPsolver.h"


int main(int argc, char *argv[]) {
if (argc>1){
srand(atoi(argv[1]));
}else{
srand(0);
}
    restart();
    return (argc);
}