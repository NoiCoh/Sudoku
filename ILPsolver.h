/*
 * ILPsolver:
 * This module uses Gurobi Optimizer Linear programming solver to solve a sudoku board.
 * This module implements integer linear programming algorithm and linear programming algorithm.
 *
 *	LP Solver:
 *	the variables represent a valid cell and value option.
 *	The target function choose a random coefficient between [1 - N] to every valid variable.This target 
 *  function was chosen after a lot of tests which showed that this function delivered the best results.
 */

#ifndef SUD_ILPSOLVER_H
#define SUD_ILPSOLVER_H
#include "gurobi_c.h"
#include "Solver.h"

/*
* The function using Gurobi Optimizer Linear programming solver to solve a sudoku board.
* if integerSol=true, the function solves the board using integer linear programming algorithm.
* if integerSol=false, the function uses linear programming algorithm in order to solve the board.
* return value : the function returns LPsol struct which contains the solution's information.
*/
LPsol* LPsolver(Game* game, bool integerSol);

/*
* The function initialize variables to be continuous for linear programming and update model.
* The function add variables to model, change objective sense to maximization and update the model.
* return value : 1 - if one of the initializations failed, 0 -  otherwise.
*/
int initLpVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype, double* objective);

/*
* The function initialize variables type as binary type for intager linear programming and update model.
* The function add variables to model, change objective sense to maximization and update the model.
* return value : 1 - if one of the initializations failed, 0 -  otherwise.
*/
int initILPVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype);

/*
* The function sums the number of legal values of all empty cells.
* return value :  the numbers of varibles that participate in the algorithm
*/
int getVarsNum(int N, Game* game);

/*
* FIRST CONSTRAINT in the linear programming constrints:
* Every cell has one value and the sum of every cell's scores is 1.0.
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int firstConstraint(Game* game, GRBenv* env, GRBmodel* model, int N, int* ind, double* val);

/*
* SECOND CONSTRAINT in the linear programming constrints:
* Each value appears once in every row of the sudoku
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int secConstraint(Game* game, GRBenv* env, GRBmodel* model, int N, int* ind, double* val);

/*
* THIRD CONSTRAINT in the linear programming constrints:
* Each value appears once in every column of the sudoku
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int thirdConstraint(Game* game, GRBenv* env, GRBmodel* model, int N, int* ind, double* val);

/*
* FORTH CONSTRAINT in the linear programming constrints:
* Each value appears once in every block of the sudoku
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int forthConstraint(Game* game,GRBenv* env, GRBmodel* model, int N, int n, int m, int* ind, double* val);

/*
* The function adding all constraints to the linear programming.
* return value : 1 if adding one of the constraint failed and 0 otherwise.
*/
int allConstraints(Game* game, GRBenv* env, GRBmodel* model, int n, int m);

/*
* The function calculate the optimize value for every variable using Gurobi Optimizer.
* return value : 1 - if the Gurobi Optimizer failed, 0 - otherwise.
*/
int optimize(GRBenv* env, GRBmodel* model);

/*
* The function free all memory resources and environment.
*/
void freeLpSolver(GRBenv* env, GRBmodel* model, char* vType, double* objective, double* values);

/*
* the function free LPsol struct.
*/
void freeLpSol(LPsol* lpSol);

/*
* The function init the mapping of every cell.
*/
void initMap(Game* game);
#endif 
