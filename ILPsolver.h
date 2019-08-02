#ifndef SUD_ILPSOLVER_H
#define SUD_ILPSOLVER_H
#include "gurobi_c.h"
#include "Solver.h"

LPsol* LPsolver(Game* game, bool intSol);

LPsol* initLpSol();

int initLpVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype, double* objective);

void initObjective(double* objective, int varsNum, bool intSol);

void initVals(double* val, int N);

void initInd(int* ind, int N);

void initVtype(int N, char* vType);

void initSol(int N, double* sol);

int initVars(GRBenv* env, GRBmodel* model, int N, char* vtype);

int getVarsNum(int N, Game* game);

void makeScores(LPsol* lpSol, double* sol, int N);

int creaetEnv(GRBenv* env, GRBmodel* model);

int calculateIndex(int row, int col, int k, int N);

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
int forthConstraint(GRBenv* env, GRBmodel* model, int N, int n, int m, int* ind, double* val);

/**
fixed cells are already known
**/
int fifthConstraint(GRBenv* env, GRBmodel* model, Board* board, int N);

int allConstraints(Game* game, GRBenv* env, GRBmodel* model, int n, int m);

/* Optimize model - need to call this before calculation */
int optimize(GRBenv* env, GRBmodel* model);

/* Get solution information */
int optimizeStatus(GRBenv* env, GRBmodel* model, int optimstatus);

/* get the solution - the assignment to each variable
 * sol is 3D matrix that includes the cell's score*/
int getSol(GRBenv* env, GRBmodel* model, int optimstatus, int varsNum, double* sol);

void updateBoard(Board* solBoard, double* sol, int N);

void freeLpSolver(GRBenv* env, GRBmodel* model, char* vType, double* sol, double* objective);
#endif 
