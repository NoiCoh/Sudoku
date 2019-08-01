#include "ILPsolver.h"
#include "Solver.h"
int LPsolver(Game* game) {
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	double *val = NULL;
	int *ind=NULL;
	double*	sol=NULL;
	int	error, *optimstatus;
	char* vType = NULL;
	int m, n, N;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = n * m;
	error = 0;
	initVtype(N, vType);
	initSol(N, sol);
	error = creaetEnv(env, model);
	if (error) {
		freeSolved(env, model, vType, sol);
		return 0;
	}
	error = initLpVars(env, model, N, vType);
	if (error) {
		freeSolved(env, model, vType, sol);
		return 0;
	}
	error = LpConstraint(env, model, N, ind, val);
	if (error) {
		freeSolved(env, model, vType, sol);
		return 0;
	}
	error = optimize(env, model);
	if (error) {
		freeSolved(env, model, vType, sol);
		return 0;
	}
	error = optimizeStatus(env, model, optimizeStatus);
	if (error) {
		freeSolved(env, model, vType, sol);
		return 0;
	}
	error = getSol(env, model, optimizeStatus, N, sol);
	if (error) {
		freeSolved(env, model, vType, sol);
		return 0;
	}
	makeScores(game,sol,N);
	freeSolved(env, model, vType, sol);
	return 1;
}

void makeScores(Game* game, double* sol, int N){
	int i, j, k, ind;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind = calculateIndex(i, j, k, N);
				game->scores[i][j][k] = sol[ind];
			}
		}
	}
}

int initLpVars(GRBenv *env, GRBmodel *model, int N, char* vtype) {
	int error;
	int i, j, k, ind;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind = calculateIndex(i, j, k, N);
				vtype[ind] = GRB_CONTINUOUS;
			}
		}
	}
}

/**
every cell's vars's sum is 1.0 
**/
int LpConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, error;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				index = calculateIndex(i, j, k, N);
				ind[k] = index;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return -1;
			}

		}
	}
	return 0;
}

void initilizeILPSolve(ILPsol * solve, Game* game) {
	solve->solBoard = initialize(game->board->blocksize);
	solve->solBoard = false;
}

ILPsol ILPsolver(Game* game) {
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	double*	sol=NULL;
	int	error,*optimstatus;
	char* vType=NULL;
	ILPsol solve;
	int m, n, N;
	m = game->board->blocksize.m;
	n = game->board->blocksize.n;
	N = n * m;
	initilizeILPSolve(&solve, game);
	error = 0;
	initVtype(N, vType);
	initSol(N, sol);
	error = creaetEnv(env, model);
	if (error) {
		freeSolved(env, model, vType, sol);
		return solve;
	}
	error=initVars(env, model, N, vType);
	if (error) {
		freeSolved(env, model, vType, sol);
		return solve;
	}
	error = allConstraints(env, model, game->board, n, m);
	if (error) {
		freeSolved(env, model, vType, sol);
		return solve;
	}
	error=optimize(env, model);
	if (error) {
		freeSolved(env, model, vType, sol);
		return solve;
	}
	error = optimizeStatus(env, model,optimizeStatus);
	if (error) {
		freeSolved(env, model, vType, sol);
		return solve;
	}
	error = getSol(env, model, optimizeStatus, N, sol);
	if (error) {
		freeSolved(env, model, vType, sol);
		return solve;
	}
	updateBoard(solve.solBoard, sol, N);
	freeSolved(env, model, vType, sol);
	solve.solvable = true;
	return solve;
}

void freeSolved(GRBenv *env, GRBmodel *model, char* vType, double * sol) {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(sol);
	free(vType);

}
int creaetEnv(GRBenv *env, GRBmodel *model) {
	unsigned int error;
	error = GRBloadenv(&env, "mip1.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Create an empty model named "mip1" */
	error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}
	return 0;
}

int initVars(GRBenv *env, GRBmodel *model, int N, char* vtype) {
	unsigned int error;
	int i, j, k,ind;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind = calculateIndex(i, j, k, N);
				vtype[ind] = GRB_BINARY;
			}
		}
	}
	/* add variables to model */
	error = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, NULL, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}
	return 0;
}
int calculateIndex(int row, int col, int k, int N) {
	return row * N*N + col * N + k;
}
void initVals(double *val,int N) {
	int i;
	val = (double*)malloc(N * sizeof(double));
	for (i = 0; i < N; i++) {
		val[i] = 1;
	}
	if (val == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
}
void initInd(int * ind, int N) {
	ind = (int*)malloc(N * sizeof(int));
	if (ind == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
}

void initVtype(int N, char* vType) {
	vType = (char*)malloc(N*N*N * sizeof(char));
	if (vType == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
}

void initSol(int N, double* sol) {
	sol = (double*)calloc(N*N*N, sizeof(double));
	if (sol == NULL) {
		printf("Error: calloc failed\n");
		exit(0);
	}
}


/**
every cell has one value
**/
int firstConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, error;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				index = calculateIndex(i, j, k, N);
				ind[k] = index;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return -1;
			}

		}
	}
	return 0;
}
/**
each value appears once in every row 
**/
int secConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, error;
	for (k = 0; k < N; k++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				index = calculateIndex(i, j, k, N);
				ind[j] = index;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return -1;
			}
			
		}
	}
	return 0;
}
/**
each value appears once in every column
**/
int thirdConstraint(GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, error;
	for (k = 0; k < N; k++) {
		for (j = 0; j< N; j++) {
			for (i = 0; i < N; i++) {
				index = calculateIndex(i, j, k, N);
				ind[i] = index;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return -1;
			}

		}
	}
	return 0;
}

/**
each value appears once in every block
**/
int forthConstraint(GRBenv *env, GRBmodel *model, int N, int n, int m, int* ind, double* val) {
	int i, j, k, index, error, jrow, jcol;
	for (jcol = 0; jcol < N; jcol + m) {
		for (jrow = 0; jrow < N; jrow + n) {
			for (k = 0; k < N; k++) {
				for (i = 0; i < m; i++) {
					for (j = 0; j < n; j++) {
						index = calculateIndex(i+jcol, j+jrow, k, N);
						ind[i*n+j] = index;
					}
					error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
					if (error) {
						printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
						return -1;
					}
				}
			}
		}
	}
	return 0;
}
/**
fixed cells are already known
**/
int fifthConstraint(GRBenv *env, GRBmodel *model, Board *board,int N) {
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
					printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return -1;
				}
			}
		}
	}
	return 0;
}

int allConstraints(GRBenv *env, GRBmodel *model, Board *board, int n, int m) {
	double *val=NULL;
	int *ind=NULL;
	int N,error ;
	N = n * m;
	initVals(val, N);
	initInd(ind, N);
	error=firstConstraint(env, model, N, ind, val);
	if (!error) {
		error = secConstraint(env, model, N, ind, val);
	}
	if (!error) {
		error = thirdConstraint(env, model, N, ind, val);
	}
	if (!error) {
		error=forthConstraint(env, model, N, n, m, ind, val);
	}
	if (!error) {
		error = fifthConstraint(env, model, board, N);
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
		return -1;
	}
	return 0;
}

	/* Get solution information */
int optimizeStatus(GRBenv *env, GRBmodel *model,int *optimstatus) {
	int error;
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	return 0;
}


/* get the solution - the assignment to each variable
 * sol is 3D matrix that includes the cell's score*/
int getSol(GRBenv *env, GRBmodel *model, int *optimstatus,int N, double *sol) {
	int error;
	if ((*optimstatus) == GRB_OPTIMAL) {
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, &sol);
		if (error) {
			printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
			return -1;
		}	
	}
	return 0;
}

void updateBoard(Board *solBoard, double *sol, int N) {
	int i, j, k,index;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				index = calculateIndex(N, i, j, k);
				if (sol[index] == 1) {
					solBoard->cells[i][j].value = k + 1;
					break;
				}
			}
		}
	}
}
