

void scalarMult (TMatrix *matrix1, TMatrix *matrix2, TMatrix *matrix_res) {
	int counter = 0;
	for(counter = 0; counter < matrix1->amount; counter++) {
		matrix_res->values[counter] = matrix1->values * matrix2->values;
	}
}

void vectorAdd (TMatrix *matrix1, TMatrix *matrix2, TMatrix *matrix_res) {
	int counter = 0;
	for(counter = 0; counter < matrix1->amount; counter++) {
		matrix_res->values[counter] = matrix1->values + matrix2->values;
	}
}

void matrixMult (TMatrix *matrix1, TMatrix *matrix2, TMatrix *matrix_res) {
	int counter = 0, innerCounter = 0;
	int subtotal = 0;
	int old1 = 0, old2 = 0;
	for(counter = 1; counter <= (matrix1->mult); counter++) {

		old2 = counter;
		for(innerCountercounter = 1; innerCountercounter <= matrix1->col; innerCounter++) {

			old1 += innerCounter
			subtotal += matrix1->values[old1] * matrix2->values[old2];
			old2 += matrix2->col;
		}
		matrix_res->values[counter] = subtotal;
	}
}

void matrixMult (TMatrix *matrix1, TMatrix *matrix2, TMatrix *matrix_res) {
	int counter = 0, innerCounter = 0;
	int subtotal = 0;
	int old1 = 0, old2 = 0;
	for(counter = 1; counter <= (matrix1->mult); counter++) {

		old2 = counter;
		for(innerCountercounter = 1; innerCountercounter <= matrix1->col; innerCounter++) {

			old1 += innerCounter
			subtotal += matrix1->values[old1] * matrix2->values[old2];
			old2 += matrix2->col;
		}
		matrix_res->values[counter] = subtotal;
	}
}

int eigth (TMatrix *matrix1, TMatrix *matrix2) {
	int exists = 0;

for



return exists
}
