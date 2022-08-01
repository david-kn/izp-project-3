#include <stdio.h>// I/O
#include <stdlib.h>// EXIT_SUCCESS a EXIT_FAILURE
#include <string.h> // strcmp()
#include <errno.h>	// ernno

// Prototypes of functions
int checkInput (char *input_str);


//Declaration of structures, enumerations, variables, ...
enum states {
	S_HELP,
	S_TEST,
	S_VADD,
	S_VSCAL,
	S_MMULT,
	S_MEXPR,
};

enum e_codes {
	E_OK,
	E_WRONG_PARAM,
	E_NOT_READABLE,
	E_WRONG_INPUT,
	E_NOT_ENOUGH_MEM,
};

const char *error_msg[] = {
	"Vse v poradku !\n",
	"Spatny vstupni parametr(y) !\n",
	"Neexistuje/nelze otevrit dany soubor pro cteni !\n",
	"Spatny vstup\n",
	"Nedostatek pameti\n",
};

typedef struct params {
	int state;
	int error;
	int matrix_num;
	char *fileName[1];
} TParams;

typedef struct matrix {
	int type;
	int amount;
	int col;
	int row;
	int *values;
	int mult;		// = collums * rows 		<= number of values
} TMatrix;

typedef struct settings {
	FILE *file[1];
	//FILE *file;
	TMatrix *matrix[2];	// pointer at the TMatrix 3-elements array; [0 -result, 1 - 1st matrix, 2nd - matrix (optional)]
	TMatrix *matrix_res;
} TSettings;

// Declaration of functios
TParams getParams (int argc, char *argv[]) {

	TParams init = {		// init structure
		.state = S_HELP,
		.error = E_OK,
		.matrix_num = 3,
	};

	if (argc >= 2 && argc <= 4) {	// process with parametres
		switch (argc) {
			case 2:
				if (strcmp(argv[1], "-h") == 0)
					init.state = S_HELP;
				else
					init.error = E_WRONG_PARAM;
				break;

			case 3:
				if (strcmp(argv[1],"--test") == 0) {
					init.state = S_TEST;
					init.fileName[0] = argv[2];
					init.matrix_num = 2;		// 1 - result; 2 - input (or many matrixes together in one array)
				}
				else
					init.error = E_WRONG_PARAM;
				break;

			case 4:
				init.matrix_num = 3;		// 1 - input file; 2 - input file; 3 - result
				if (strcmp(argv[1],"--vadd") == 0)		 // processing 1nd parametr
					init.state = S_VADD;
				else if (strcmp(argv[1], "--vscal") == 0)
					init.state = S_VSCAL;
				else if (strcmp(argv[1],"--mmult") == 0)
					init.state = S_MMULT;
				else if (strcmp(argv[1], "--mexpr") == 0)
					init.state = S_MEXPR;
				else
					init.error = E_WRONG_PARAM;
				if (init.error == E_OK) {		// saving filenames to structure if is everytihing OK so far...
						init.fileName[0] = argv[2];
						init.fileName[1] = argv[3];
				}
				break;
		}
	}
	else if (argc == 6) { // maybe snooker

	}
	else {	// wrong param(s)
		init.error = E_WRONG_PARAM;
	}
return init;
}

int checkFiles (TParams *p_params, TSettings *p_settings, int argc) {	//tries to open file (reading mode only)
	int ok = 1, counter = 0;

	if((p_settings->file[counter] = fopen(p_params->fileName[counter], "r")) == NULL)
		ok = 0;
	if (argc != 3) {	// case where 2 files are set in parametres -. 2 files have to be open
		counter++;		// increasing of counter of array
		if((p_settings->file[counter] = fopen(p_params->fileName[counter], "r")) == NULL)
			ok = 0;
	}
	if(!ok)
		p_params->error = E_NOT_READABLE;

return ok;
}

int readHeadFromFile(TParams *p_params, TSettings *p_settings) {
	int counter = 0, MCounter = 0, ok = 1;
	TMatrix *matrix;
	int times = 2;		// how many times shuld take the input as the setting values [1 = vector; 2 = matrix; 3 = vector of matrixes]
	char *set[3];
	char *q;

	for(MCounter = 0; MCounter < (p_params->matrix_num - 1); MCounter++) {	// '-1' because a result matrix is defined elsewhere
		matrix = p_settings->matrix[MCounter];
		q = &(set)[counter];
		fscanf(p_settings->file[MCounter], "%s", q); // read 1st integer value - type of input
		printf("%s\n", q);
		ok = checkInput(q);
		matrix->type = ok;
		times = ok;
		set[counter] = ok;
		//times = ok[counter];	// count

		// reading the settings of matrix/vector
		for (counter = 1; (times > 0 && counter < (times+1) && times < 4); counter++) {
			q = &(set)[counter];
			fscanf(p_settings->file[MCounter], "%s", q); // read 1st integer value - type of input
			printf("%s\n", q);
			ok = checkInput(q);
			/*fscanf(p_settings->file, "%s", q);
			printf("%s\n", q);
			ok = checkInput(q);*/
			if(errno == ERANGE) {		// bad input value
				p_params->error = E_WRONG_INPUT;
				ok = 0;
				break;
			}
			//times = setNumber(p_settings->matrix);
		}
		if (times > 0 && times < 4) {
			counter = 0;
			int test = set[counter];
			printf("Test:%d\n", test);
			matrix->type = set[counter]; // 0; in variable 'ok' is the the first value of input which sets the structure of rest of the input
			p_params->matrix_num = 1;	// general setting
			if (matrix->type == 3) {// if case of 'vector of matrix' - overwrite previous setting
				p_params->matrix_num = set[counter++];					// position: 1;
				matrix->amount = set[counter]; 					//the same data
			}
			matrix->row = (int)set[counter++];	// pos.: 1/2;
			int tp = matrix->row;
			printf("TP:%d\n", tp);
			matrix->col = set[counter++];	// pos.: 2/3;
			tp = matrix->col;
			printf("TP:%d\n", tp);
			matrix->mult = (matrix->col * matrix->row);
			tp = matrix->mult;
			printf("TP:%d\n", tp);
		}
		else {
			p_params->error = E_WRONG_INPUT;
			break;
		}
		counter = 0;		// because of loading of the others matrix in next 'for'
	}
return ok;
}

int checkInput (char *input_str) {
	int input_value;
	int counter = 0;

	input_value = atoi(input_str);
	if(!input_value/* && (((int)input_str[0]) != 48)*/) {
		input_value = 0;
		errno = ERANGE;
	}

	return input_value;
}

int allocMemory (TParams *p_params, TSettings *p_settings) {
	int ok = 1, counter = 0;

		if (p_params->state != 1) {	// 3 matrixes
			if(p_settings->matrix[counter]->values = malloc(p_settings->matrix[counter]->mult * sizeof(int)) == NULL)
				ok = 0;
			if(p_settings->matrix[counter+1]->values = malloc(p_settings->matrix[counter+1]->mult * sizeof(int)) == NULL)
				ok = 0;
		}
		else if (p_settings->matrix[counter]->type != 3)	{// state == S_TEST - 1 matrixes
			// nothing need to be done
		}
		else  {	//		 2 matrixes // 3rd type !!
			if(p_settings->matrix[counter]->values = malloc(p_settings->matrix[counter]->mult * p_params->matrix_num * sizeof(int)) == NULL)
				ok = 0;
		}
		if(p_settings->matrix_res->values = malloc(p_settings->matrix_res->mult * sizeof(int)) == NULL)
			ok = 0;

		if(!ok)
			p_params->error = E_NOT_ENOUGH_MEM;
return ok;
}

int loadMatrix (TParams *p_params, TSettings *p_settings, int counter) {
	TMatrix *matrix;
	matrix = p_settings->matrix[counter];
	int ok = 1;
	int inner_counter = 0;
	char *input_value;
	char *tp;
	tp = &input_value;
	int limit;
	limit = matrix->mult;

	printf("M->m:%d\n", matrix->mult);
	while(counter < (matrix->amount) && counter < 10) {
		printf("M->a:%d\n", matrix->amount);
		for (inner_counter; inner_counter < limit && inner_counter < 5; inner_counter++) {

			fscanf(p_settings->file[counter], "%s", tp);
			printf("%s\n", tp);
			input_value = checkInput(tp);
		}
		//inner_counter += (matrix->col * matrix->row);
		limit += matrix->mult;
		counter++;
	}
return ok;
}

void initMatrix(TParams *p_params, TSettings *p_settings) {
	int counter = 0;

	for(counter = 0; counter < p_params->matrix_num; counter++) {
		TMatrix matrix;
		matrix.amount = 1;
		p_settings->matrix[counter] = &matrix;

	}
	TMatrix matrix_res;		// init of 'result matrix'
	p_settings->matrix_res = &matrix_res;
}

int controlProcedure (TParams *p_params, TSettings *p_settings, int argc) {	// tries to open file (reading mode only) and alloc enough memory
	int ok = 1, counter = 0;

	//ok = initMatrix(p_settings, p_params->matrix_num);
	if(ok = checkFiles(p_params, p_settings, argc)) {
		printf("checkFiles\n");
		initMatrix(p_params, p_settings);
		printf("initMatrix\n");
		if(ok = readHeadFromFile(p_params, p_settings)) {
			printf("po cteni hlavicky\n");
			if(ok = allocMemory(p_params, p_settings)) {
				printf("allocMem\n");
 				while(p_settings->file[counter] != NULL) {
					loadMatrix(p_params, p_settings, counter);
					counter++;
					}
//			ok = readHeadFromFile(p_params, p_settings);
			}
		}
	}
return ok;
}

void writeMatrix (TParams *p_params, TSettings *p_settings) {
	int counter = 0, inner_counter = 0;
	TMatrix *matrix;
	matrix = p_settings->matrix[counter];

	for (counter = 0; counter < matrix->mult; counter++) {
		printf("  %d  ", matrix->values[counter]);
		if((counter % matrix->col) == 0)
			printf("\n");
	}
}

int freeMemory (TSettings *p_settings) {
	free(p_settings->matrix_res->values);
	free(p_settings->matrix[0]->values);
}
//

void print_error (int code) {
	fprintf(stderr, "%s", error_msg[code]);
}




int main(int argc, char *argv[])
{
	int ok = 1;
	int counter = 0;
	// Initialization of structures
	TParams params = getParams(argc, argv);	// procedig of input parametres
	TParams *p_params = &params;
	TSettings settings;
	TSettings *p_settings = &settings;

	// Preparing procedure - check & open file; check data and load them into the heap
	if(params.error != 0 || !controlProcedure(p_params, p_settings, argc)) {
		print_error(params.error);
		return EXIT_FAILURE;
	}

	writeMatrix(p_params, p_settings);

	freeMemory(p_settings);

	// preparingProcedure();
	/*
	if (params.error != 0 || (checkFiles(p_params, p_settings, argc) == 0))	{	// error was found during processing input parametres
		print_error(params.error);
		return EXIT_FAILURE;
	}
	if(!(loadMatrix(p_settings)))
		return EXIT_FAILURE;
*/

////////////****************************************************
////////////**************************************************** DELETE NEW LINE PRINTF
	printf("\n");
////////////****************************************************
////////////**************************************************** DELETE NEW LINE PRINTF
    return EXIT_SUCCESS;
}
