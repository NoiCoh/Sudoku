#include "ILPsolver.h"

LPsol* LPsolver(Game* game,bool intSol) {
	int m, n, N, error, varsNum, optimstatus;
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
	lpSol = initLpSol();
	varsNum = getVarsNum(N, game);
	initVtype(varsNum, vType);
	initSol(varsNum, sol);
	initObjective(objective, varsNum,intSol);
	error = creaetEnv(env, model);
	if (!error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	if (intSol == true) {
		error = initVars(env, model, N, vType);
	}
	else {
		error = initLpVars(env, model, N, vType, objective);
	}
	if (!error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	error = allConstraints(game, env, model, n, m);
	if (!error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	error = optimize(env, model);
	if (!error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	error = optimizeStatus(env, model, optimstatus);
	if (!error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	error = getSol(env, model, optimstatus, varsNum, sol);
	if (!error) {
		freeLpSolver(env, model, vType, sol, objective);
		return lpSol;
	}
	if (optimstatus == GRB_OPTIMAL) {
		lpSol->solvable = 1;
		if (intSol == true) {
			updateBoard(lpSol->solBoard, sol, N);
		}
	}
	makeScores(lpSol,sol,N);
	freeLpSolver(env, model, vType, sol, objective);
	return lpSol;
}

LPsol* initLpSol() {
	LPsol* lpSol = malloc(sizeof(LPsol));
	if (!lpSol) {
		printf("Error: malloc failed");
		exit(0);
	}
	lpSol->solvable = 0;
	lpSol->scores = NULL;
	return lpSol;
}

int initLpVars(GRBenv* env, GRBmodel* model, int varsNum, char* vtype, double* objective) {
	int error, i;
	for (i = 0; i < varsNum; i++) {
		vtype[i] = GRB_CONTINUOUS;
	}

	/* add variables to model */
	error = GRBaddvars(model, varsNum, 0, NULL, NULL, NULL, objective, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}

void initObjective(double* objective, int varsNum,bool intSol) {
	int i ;
	objective = malloc(varsNum * sizeof(double));
	if (!objective) {
		printf("Error: malloc failed");
		exit(0);
	}
	for (i = 0; i < varsNum; i++) {
		if (intSol == true) {
			objective[i] = 0;
		}
		else {
			objective[i] = 1.0 + rand() % 10;
		}
	}
	
}

void initVals(double* val, int N) {
	val = (double*)malloc(N * sizeof(double));
	if (val == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
}
void initInd(int* ind, int N) {
	ind = (int*)malloc(N * sizeof(int));
	if (ind == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
}

void initVtype(int N, char* vType) {
	int N3 = N * N * N;
	vType = (char*)malloc(N3 * sizeof(char));
	if (vType == NULL) {
		printf("Error: malloc failed\n");
		exit(0);
	}
}

void initSol(int N, double* sol) {
	int N3 = N * N * N;
	sol = (double*)calloc(N3, sizeof(double));
	if (sol == NULL) {
		printf("Error: calloc failed\n");
		exit(0);
	}
}


int initVars(GRBenv* env, GRBmodel* model, int N, char* vtype) {
	unsigned int error;
	int i, j, k, ind;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind = calculateIndex(i, j, k, N);
				vtype[ind] = GRB_BINARY;
			}
		}
	}
	/* add variables to model */
	error = GRBaddvars(model, N * N * N, 0, NULL, NULL, NULL, NULL, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}

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


void makeScores(LPsol* lpSol, double* sol, int N){
	int i, j, k, ind;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind = calculateIndex(i, j, k, N);
				lpSol->scores[i][j][k] = sol[ind];
			}
		}
	}
}


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

int calculateIndex(int row, int col, int k, int N) {
	return row * N*N + col * N + k;
}


/**
every cell has one value and every cell's scores's sum is 1.0 
**/
int firstConstraint(Game* game, GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, v,error,varsNum;
	varsNum = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < game->board->cells[i][j].optionsSize; k++) {
				v = game->board->cells[i][j].options[k];
				index = calculateIndex(i, j, v, N);
				ind[varsNum] = index;
				val[varsNum] = 1.0;
				varsNum++;
			}
			if (varsNum == 0) {
				printf("Error:No legal value for cell <%d,%d>", i + 1, j + 1);
				return 0;
			}
			error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 1nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return 0;
			}
		}
	}
	return 1;
}
/**
each value appears once in every row 
**/
int secConstraint(Game* game ,GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, error, varsNum;
	bool found;
	found = false;
	for (k = 1; k < N; k++) {
		for (i = 0; i < N; i++) {
			varsNum = 0;
			for (j = 0; j < N; j++) {
				if (game->board->cells[i][j].value == k) {
					found = true;
					break;
			
				}
				if (found == true) {
					index = calculateIndex(i, j, k, N);
					ind[varsNum] = index;
					val[varsNum] = 1.0;
					varsNum++;
				}
			}
			if (found == true) {
				found = false;
				break;
			}
			error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return 0;
			}
			
		}
	}
	return 1;
}
/**
each value appears once in every column
**/
int thirdConstraint(Game* game,GRBenv *env, GRBmodel *model, int N, int* ind, double* val) {
	int i, j, k, index, error, varsNum;
	bool found;
	found = false;
	for (k = 1; k < N; k++) {
		for (j = 0; j < N; j++) {
			varsNum = 0;
			for (i = 0; i < N; i++) {
				if (game->board->cells[i][j].value == k) {
					found = true;
					break;

				}
				if (found == true) {
					index = calculateIndex(i, j, k, N);
					ind[varsNum] = index;
					val[varsNum] = 1.0;
					varsNum++;
				}
			}
			if (found == true) {
				found = false;
				break;
			}
			error = GRBaddconstr(model, varsNum, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 3nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return 0;
			}

		}
	}
	return 1;
}

/**
each value appears once in every block
**/
int forthConstraint(GRBenv *env, GRBmodel *model, int N, int n, int m, int* ind, double* val) {
	int r, c, k, i, j, varsNum, index, error;
	for (r = 0; r < n; r++) {
		for (c = 0; c < m; c++) {
			for (k = 1; k <= N; k++) {
				varsNum = 0;
				for (i = r * m; i < (r + 1)*m; i++) {
					for (j = c * n; j < (c + 1)*n; j++) {
						index = calculateIndex(i, j, k, N);
						ind[varsNum] = index;
						val[varsNum] = 1.0;
						varsNum++;
					}
				}
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 4nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				return 0;
			}
		}
	}
	return 1;
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
					printf("ERROR %d 5nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 0;
				}
			}
		}
	}
	return 1;
}

int allConstraints(Game* game, GRBenv *env, GRBmodel *model, int n, int m) {
	double *val=NULL;
	int *ind=NULL;
	int N,error ;
	N = n * m;
	initVals(val, N);
	initInd(ind, N);
	error=firstConstraint(game ,env, model, N, ind, val);
	if (!error) {
		error = secConstraint(game, env, model, N, ind, val);
	}
	if (!error) {
		error = thirdConstraint(game, env, model, N, ind, val);
	}
	if (!error) {
		error=forthConstraint(env, model, N, n, m, ind, val);
	}
	if (!error) {
		error = fifthConstraint(env, model,game->board, N);
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
		return 0;
	}
	return 1;
}

	/* Get solution information */
int optimizeStatus(GRBenv *env, GRBmodel *model,int optimstatus) {
	int error;
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	return 1;
}


/* get the solution - the assignment to each variable
 * sol is 3D matrix that includes the cell's score*/
int getSol(GRBenv *env, GRBmodel *model, int optimstatus,int varsNum, double *sol) {
	int error;
	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varsNum, sol);
		if (error) {
			printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
			return 0;
		}	
	}
	return 1;
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
void freeLpSolver(GRBenv* env, GRBmodel* model, char* vType, double* sol, double* objective) {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(sol);
	free(vType);
	free(objective);
}