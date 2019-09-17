/*
 * ILPsolver:
 * This module uses Gurobi Optimizer Linear programming solver to solve a sudoku board.
 * This module implements integer linear programming algorithm and linear programming algorithm.
 *
 *	LP Solver:
 *	the variables represent a valid cell and value option.
 *	The target function choose a random coefficient between [1 - N] to every valid variable.
 */
#include "ILPsolver.h"

/*
* The function using Gurobi Optimizer Linear programming solver to solve a sudoku board.
* if integerSol=true, the function solves the board using integer linear programming algorithm.
* if integerSol=false , the function uses linear programming algorithm in order to solve the board.
* return value : the function returns LPsol struct which contains the solution's information.
*/
LPsol* LPsolver(Game* game, bool integerSol) {
	int m, n, N, error, varsNum, optimstatus, i;
	double* objective = NULL;
	LPsol* lpSol;
	GRBenv* env = NULL;
	GRBmodel* model = NULL;
	double* values = NULL;
	char* vType = NULL;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = n * m;
	/*---------initilaize variables---------*/
	error = 0;
	optimstatus = 0;
	initMap(game);
	lpSol = (LPsol*)malloc(sizeof(LPsol));
	if (!lpSol) {
		funcFailed("malloc");
	}
	lpSol->solvable = 0;
	lpSol->solBoard = NULL;
	varsNum = getVarsNum(N, game);
	vType = (char*)malloc(varsNum * sizeof(char));
	if (vType == NULL) {
		funcFailed("malloc");
	}
	values = (double*)calloc(varsNum, sizeof(double));
	if (values == NULL) {
		funcFailed("calloc");
	}
	objective = (double*)calloc(varsNum, sizeof(double));
	if (!objective) {
		funcFailed("calloc");
	}
	if (integerSol != true) {
		for (i = 0; i < varsNum; i++) {
			objective[i] = 1.0 + rand() % N;
		}
	}
	/*----------------------------------*/
	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	/* Create an empty model named "sudoku" */
	error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	}
	if (error) {
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	if (integerSol == true) {
		error = initILPVars(env, model, varsNum, vType);
	} else {
		error = initLpVars(env, model, varsNum, vType, objective);
	}
	if (error) {
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	error = allConstraints(game, env, model, n, m);
	if (error) {
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	error = optimize(env, model);
	if (error) {
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, objective);
	if (error) {
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varsNum, values);
	if (error) {
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		freeLpSolver(env, model, vType, objective, values);
		return lpSol;
	}
	if (optimstatus == GRB_OPTIMAL) {
		lpSol->solvable = 1;
		lpSol->solBoard = (double*)calloc(varsNum, sizeof(double));
		if (lpSol->solBoard == NULL) {
			funcFailed("calloc");
		}
		for (i = 0; i < varsNum; i++) {
			lpSol->solBoard[i] = values[i];
		}
	}
	freeLpSolver(env, model, vType, objective, values);
	return lpSol;
}

/*
* The function initialize variables to be continuous for linear programming and update model.
* The function add variables to model, change objective sense to maximization and update the model.
* return value : 1 - if one of the initializations failed, 0 -  otherwise. 
*/
int initLpVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype, double* objective) {
	int error, i;
	for (i = 0; i < varsNum; i++) {
		vtype[i] = GRB_CONTINUOUS;
	}
	/* add variables to model */
	error = GRBaddvars(model, varsNum, 0, NULL, NULL, NULL, objective, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	/* update the model - to integrate new variables */
	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	return 0;
}

/*
* The function initialize variables type as binary type for intager linear programming and update model.
* The function add variables to model, change objective sense to maximization and update the model.
* return value : 1 - if one of the initializations failed, 0 -  otherwise.
*/
int initILPVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype) {
	unsigned int error;
	int i;
	for (i = 0; i < varsNum; i++) {
		vtype[i] = GRB_BINARY;
	}
	/*add variables to model*/
	error = GRBaddvars(model, varsNum, 0, NULL, NULL, NULL, NULL, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	/*Change objective sense to maximization*/
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	/*update the model - to integrate new variables*/
	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	return 0;
}

/*
* The function sums the number of legal values of all empty cells.
* return value :  the numbers of varibles that participate in the algorithm
*/
int getVarsNum(int N, Game* game) {
	int i, j, varsNum;
	index ind;
	varsNum = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			ind.row = i;
			ind.col = j;
			findOptionsCell(game, ind);
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (game->board->cells[i][j].value == 0) {
				varsNum += game->board->cells[i][j].optionsSize;
			}
		}
	}
	return varsNum;
}

/*
* FIRST CONSTRAINT in the linear programming constrints:
* Every cell has one value and the sum of every cell's scores is 1.0.
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int firstConstraint(Game* game, GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, nums, valIndex, error, v, varsNum;
	index ix;
	valIndex = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			varsNum = 0;
			if (game->board->cells[i][j].value == 0) {
				ix.row = j;
				ix.col = i;
				findOptionsCell(game, ix);
				nums = game->board->cells[i][j].optionsSize;
				if (nums == 0) {
					/* No legal value for cell */
					return 1;
				}
				for (k = 0; k < nums; k++) {
					v = game->board->cells[i][j].options[k];
					ind[varsNum] = valIndex;
					game->board->cells[i][j].ixMap[v - 1] = valIndex + 1;
					val[varsNum] = 1.0;
					varsNum++;
					valIndex++;
				}
				error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d 1nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 1;
				}
			}
		}
	}
	return 0;
}
/*
* SECOND CONSTRAINT in the linear programming constrints:
* Each value appears once in every row of the sudoku
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int secConstraint(Game* game ,GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, error, varsNum;
	bool found;
	found = false;
	for (k = 1; k <= N; k++) {
		for (i = 0; i < N; i++) {
			varsNum = 0;
			for (j = 0; j < N; j++) {
				if (game->board->cells[i][j].value == k) {
					found = true;
					break;
				}
				if (game->board->cells[i][j].ixMap[k-1] != 0) {
					ind[varsNum] = (game->board->cells[i][j].ixMap[k - 1]) - 1;
					val[varsNum] = 1.0;
					varsNum++;
				}
			}
			if ((found == true) || (varsNum == 0)) {
				found = false;
				continue;
			}
			error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return 1;
			}
			
		}
	}
	return 0;
}

/*
* THIRD CONSTRAINT in the linear programming constrints:
* Each value appears once in every column of the sudoku
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int thirdConstraint(Game* game,GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, error, varsNum;
	bool found;
	found = false;
	for (k = 1; k <= N; k++) {
		for (j = 0; j < N; j++) {
			varsNum = 0;
			for (i = 0; i < N; i++) {
				if (game->board->cells[i][j].value == k) {
					found = true;
					break;
				}
				if (game->board->cells[i][j].ixMap[k - 1] != 0) {
					ind[varsNum] = (game->board->cells[i][j].ixMap[k - 1])- 1 ;
					val[varsNum] = 1.0;
					varsNum++;
				}
			}
			if ((found == true) || (varsNum == 0)) {
				found = false;
				continue;
			}
			error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 3nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return 1;
			}

		}
	}
	return 0;
}

/*
* FORTH CONSTRAINT in the linear programming constrints:
* Each value appears once in every block of the sudoku
* return value : 0 if adding the constraint succeed and 1 otherwise.
*/
int forthConstraint(Game* game, GRBenv *env, GRBmodel *model, int N, int n, int m, int* ind, double* val) {
	int r, c, k, i, j, varsNum, error;
	bool found;
	found = false;
	for (r = 0; r < n; r++) {
		for (c = 0; c < m; c++) {
			for (k = 1; k <= N; k++) {
				varsNum = 0;
				for (i = r * m; i < (r + 1)*m; i++) {
					for (j = c * n; j < (c + 1)*n; j++) {
						if (game->board->cells[i][j].value == k) {
							found = true;
							break;
						}
						if (game->board->cells[i][j].ixMap[k - 1] != 0) {
							ind[varsNum] = (game->board->cells[i][j].ixMap[k - 1]) - 1 ;
							val[varsNum] = 1.0;
							varsNum++;
						}
					}
				}
				if ((found == true) || (varsNum == 0)) {
					found = false;
					continue;
				}
				error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d 4nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
* The function adding all constraints to the linear programming.
* return value : 1 if adding one of the constraint failed and 0 otherwise.
*/
int allConstraints(Game* game, GRBenv *env, GRBmodel *model, int n, int m) {
	double *val=NULL;
	int *ind=NULL;
	int N,error ;
	N = n * m;
	val = (double*)malloc(N * sizeof(double));
	if (val == NULL) {
		funcFailed("malloc");
	}
	ind = (int*)malloc(N * sizeof(int));
	if (ind == NULL) {
		funcFailed("malloc");
	}
	error=firstConstraint(game ,env, model, N, ind, val);
	if (!error) {
		error = secConstraint(game, env, model, N, ind, val);
	}
	if (!error) {
		error = thirdConstraint(game, env, model, N, ind, val);
	}
	if (!error) {
		error=forthConstraint(game,env, model, N, n, m, ind, val);
	}
	free(val);
	free(ind);
	return error;
}

/*
* The function calculate the optimize value for every variable using Gurobi Optimizer.
* return value : 1 - if the Gurobi Optimizer failed, 0 - otherwise.
*/
int optimize(GRBenv *env, GRBmodel *model) {
	int error;
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	return 0;
}


/*
* The function free all memory resources and environment.
*/
void freeLpSolver(GRBenv* env, GRBmodel* model, char* vType, double* objective, double* values) {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(vType);
	free(objective);
	free(values);
}

/*
* the function free LPsol struct.
*/
void freeLpSol(LPsol* lpSol) {
	free(lpSol->solBoard);
	free(lpSol);
}

/*
* The function init the mapping of every cell.
*/
void initMap(Game* game) {
	int i, j, v, N;
	N = calculateNfromGame(game);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (v = 0; v < N; v++) {
				game->board->cells[i][j].ixMap[v] = 0;
			}
		}
	}
}