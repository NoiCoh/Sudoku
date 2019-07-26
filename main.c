#include <stdio.h>
#include "Parser.c"
#include "Game.c"
#include "Solver.c"
#include "MainAux.c"
#include "ILPsolver.c"


int main(int argc, char *argv[]) {
if (argc>1){
srand(atoi(argv[1]));
}else{
srand(0);
}
    restart();
    return (argc);
}
