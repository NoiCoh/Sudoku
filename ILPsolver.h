#ifndef SUD_ILPSOLVER_H
#define SUD_ILPSOLVER_H
#include "gurobi_c.h"
#include "Solver.h"

/*
* if integerSol=true, the function solves the board using integer linear programming algorithm.
* if integerSol=false , the function uses linear programming algorithm in order to solve the board.
* the function returns LPsol struct which contains the solution's information.
*/
LPsol* LPsolver(Game* game, bool integerSol);

/*
* initialize variables to be continuous and update model.
*/
int initLpVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype, double* objective);

/*
* the function creates the objective function.
* if integerSol=true, the objective function is zero .
* if integerSol=false , each varible get a random weight from 1 to 10.
*/
void initObjective(double* objective, int varsNum, bool integerSol);

/*
* initialize variables type as binary type and update model.
*/
int initVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype);

/*
* this function returns the numbers of varibles that participate in the algorithm by counting all the legal values that can be filled in the empty cells.
*/
int getVarsNum(int N, Game* game);

/*
* creates environment -"sudoku.log".
*/
int creaetEnv(GRBenv* env, GRBmodel* model);

/**
every cell has one value and every cell's scores's sum is 1.0
**/
int firstConstraint(Game* game, GRBenv* env, GRBmodel* model, int N, int* ind, double* val);

/**
each value appears once in every row
**/
int secConstraint(Game* game, GRBenv* env, GRBmodel* model, int N, int* ind, double* val);

/**
each value appears once in every column
**/
int thirdConstraint(Game* game, GRBenv* env, GRBmodel* model, int N, int* ind, double* val);

/**
each value appears once in every block
**/
int forthConstraint(Game* game,GRBenv* env, GRBmodel* model, int N, int n, int m, int* ind, double* val);

/**
fixed cells are already known
**/
int fifthConstraint(GRBenv* env, GRBmodel* model, Board* board, int N);

/**
check if all consraints exist.
**/
int allConstraints(Game* game, GRBenv* env, GRBmodel* model, int n, int m);

/* Optimize model - need to call this before calculation */
int optimize(GRBenv* env, GRBmodel* model);

/* Get solution information */
int optimizeStatus(GRBenv* env, GRBmodel* model, int optimstatus);

/* get the solution - the assignment to each variable
 * sol is 3D matrix that includes the cell's scores
 */
int getSol(GRBenv* env, GRBmodel* model, int optimstatus, int varsNum, double* sol);

/*
* free all memory resources and environment.
*/
void freeLpSolver(GRBenv* env, GRBmodel* model, char* vType, double* sol, double* objective);

/*
*calculate the index of a varible participating in the LP algorithm.
*/
int calculateIndex(int row, int col, int k, int N);


/*--------allocate memory functions-------- */ 

void initVals(double* val, int N);

void initInd(int* ind, int N);

void initVtype(int varNums, char* vType);

void initSol(int N, double* sol);
#endif 
