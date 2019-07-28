
#ifndef SUD_ILPSOLVER_H
#define SUD_ILPSOLVER_H
#include "gurobi_c.h"
#include "Solver.h"


int LPsolver(Game* game);
void initSol(int N, double* sol);
int initLpVars(GRBenv *env, GRBmodel *model, int N, char* vtype);
int LpConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val);
ILPsol ILPsolver(Game* game);
void makeScores(Game* game, double* sol, int N);
void freeSolved(GRBenv *env, GRBmodel *model, char* vType, double * sol);
int creaetEnv(GRBenv *env, GRBmodel *model);
int initVars(GRBenv *env, GRBmodel *model, int N, char* vtype);
int calculateIndex(int row, int col, int k, int N);
void initVtype(int N, char* vType);
int firstConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val);
int secConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val);
/**
each value appears once in every column
**/
int thirdConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val);
/**
each value appears once in every block
**/
int forthConstraint(GRBenv *env, GRBmodel *model, int N, int n, int m, int* ind, double* val);
/**
fixed cells are already known
**/
int fifthConstraint(GRBenv *env, GRBmodel *model, Board *board, int N);
int allConstraints(GRBenv *env, GRBmodel *model, Board *board, int n, int m);
/* Optimize model - need to call this before calculation */
int optimize(GRBenv *env, GRBmodel *model);
/* Get solution information */
int optimizeStatus(GRBenv *env, GRBmodel *model, int *optimstatus);
/* get the solution - the assignment to each variable */
int getSol(GRBenv *env, GRBmodel *model, int *optimstatus, int N, double *sol);
void updateBoard(Board *solBoard, double *sol, int N);
void initVals(double *val, int N);
void initInd(int * ind, int N);
#endif 
