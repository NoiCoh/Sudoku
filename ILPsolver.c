#include "ILPsolver.h"

/*
* if integerSol=true, the function solves the board using integer linear programming algorithm.
* if integerSol=false , the function uses linear programming algorithm in order to solve the board.
* the function returns LPsol struct which contains the solution's information.
*/
LPsol* LPsolver(Game* game,bool integerSol) {
	int m, n, N, error, varsNum, optimstatus, i;
	double* objective=NULL;
	LPsol* lpSol;
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	double*	sol = NULL;
	char* vType = NULL;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = n * m;
	error = 0;
	optimstatus = 0;
	lpSol = (LPsol *)malloc(sizeof(LPsol));
	if (!lpSol) {
		funcFailed("malloc");
	}
	lpSol->solvable = 0;
	lpSol->solBoard= NULL;

	varsNum = getVarsNum(N, game);
	vType = (char*)malloc(varsNum * sizeof(char));
	if (vType == NULL) {
		funcFailed("malloc");
	}
	sol = (double*)calloc(varsNum, sizeof(double));
	if (sol == NULL) {
		funcFailed("calloc");
	}
	objective = (double*)malloc(varsNum * sizeof(double));
	if (!objective) {
		funcFailed("calloc");
	}
	for (i = 0; i < varsNum; i++) {
		if (integerSol == true) {
			objective[i] = 0;
		}
		else {
			objective[i] = 1.0 + rand() % 10;
		}
	}

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
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	if (integerSol == true) {
		error = initVars(env, model, varsNum, vType);
	} else {
		error = initLpVars(env, model, varsNum, vType, objective);
	}
	if (error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	error = allConstraints(game, env, model, n, m);
	if (error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}

	error = optimize(env, model);
	if (error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, objective);
	if (error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}

	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varsNum, sol);
	if (error) {
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	if (optimstatus == GRB_OPTIMAL) {
		lpSol->solvable = 1;
		lpSol->solBoard = (double*)calloc(varsNum, sizeof(double));
		if (lpSol->solBoard == NULL) {
			funcFailed("calloc");
		}
		for (i = 0; i < varsNum; i++) {
			lpSol->solBoard[i] = sol[i];
		}
	}

	freeLpSolver(env, model, vType, sol, objective);
	return lpSol;
}

/*
* initialize variables to be continuous and update model.
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
* the function creates the objective function.
* if integerSol=true, the objective function is zero .
* if integerSol=false , each varible get a random weight from 1 to 10.
*/
void initObjective(double* objective, int varsNum,bool integerSol) {
	int i ;
	objective = malloc(varsNum * sizeof(double));
	if (!objective) {
		funcFailed("calloc");
	}
	for (i = 0; i < varsNum; i++) {
		if (integerSol == true) {
			objective[i] = 0;
		}
		else {
			objective[i] = 1.0 + rand() % 10;
		}
	}
}

/*
* initialize variables type as binary type and update model.
*/
int initVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype) {
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
* this function returns the numbers of varibles that participate in the algorithm by counting all the legal values that can be filled in the empty cells.
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
* creates environment -"sudoku.log".
*/
int creaetEnv(GRBenv *env, GRBmodel *model) {
	int error;
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
		return 0;
	}
	return 1;
}


/**
every cell has one value and every cell's scores's sum is 1.0 
**/
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
					printf("Error: No legal value for cell <%d,%d> \n", i + 1, j + 1);
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
/**
each value appears once in every row 
**/
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

/**
each value appears once in every column
**/
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

/**
each value appears once in every block
**/
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
/**
fixed cells are already known
**/
/*int fifthConstraint(GRBenv *env, GRBmodel *model, Board *board,int N) {
	int i, j, k, index, error;
	int ind[1];
	double val[1];
	val[0] = 1;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board->cells[i][j].fixed==true){
				k = board->cells[i][j].value;
				index = calculateIndex(i, j, k, N);
				ind[0] = index;
				error = GRBaddconstr(model, 1, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d 5nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 1;
				}
			}
		}
	}
	return 0;
}*/

/**
check if all consraints exist.
**/
int allConstraints(Game* game, GRBenv *env, GRBmodel *model, int n, int m) {
	double *val=NULL;
	int *ind=NULL;
	int N,error ;
	N = n * m;
	/*initVals(val, N);*/
	val = (double*)malloc(N * sizeof(double));
	if (val == NULL) {
		funcFailed("malloc");
	}
	/*initInd(ind, N);*/
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

/* Optimize model - need to call this before calculation */
int optimize(GRBenv *env, GRBmodel *model) {
	int error;
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	return 0;
}

/* Get solution information */
/*int optimizeStatus(GRBenv *env, GRBmodel *model,int optimstatus) {
	int error;
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 1;
	}
	return 0;
}*/

/* get the solution - the assignment to each variable
 * sol is 3D matrix that includes the cell's score*/
int getSol(GRBenv *env, GRBmodel *model, int optimstatus,int varsNum, double *sol) {
	int error;
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varsNum, sol);
		if (error) {
			printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
			return 1;
		}	
	}
	return 0;
}

/*
* free all memory resources and environment.
*/
void freeLpSolver(GRBenv* env, GRBmodel* model, char* vType, double* sol, double* objective) {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(sol);
	free(vType);
	free(objective);
}

/*
*calculate the index of a varible participating in the LP algorithm.
*/
int calculateIndex(int row, int col, int k, int N) {
	int res;
	res = k + N * (col + N * row);
	return res;
}

/*--------allocate memory functions-------- */

void initVals(double* val, int N) {
	val = (double*)malloc(N * sizeof(double));
	if (val == NULL) {
		funcFailed("malloc");
	}
}

void initInd(int* ind, int N) {
	ind = (int*)malloc(N * sizeof(int));
	if (ind == NULL) {
		funcFailed("malloc");
	}
}

void initVtype(int varNums, char* vType) {
	vType = (char*)malloc(varNums * sizeof(char));
	if (vType == NULL) {
		funcFailed("malloc");
	}
}

void initSol(int N, double* sol) {
	int N3 = N * N * N;
	sol = (double*)calloc(N3, sizeof(double));
	if (sol == NULL) {
		funcFailed("calloc");
	}
}