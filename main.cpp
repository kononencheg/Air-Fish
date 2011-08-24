/**
 * Air-Fish
 *
 * @file main.cpp
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>
#include <iostream>

#include "math/state_space.h"
#include "math/step_signal.h"

int main() {
	Matrix A(2, 2);
	Matrix B(2, 1);
	Matrix C(2, 2);

	A(0,0) = 1;
	A(0,1) = 10;
	A(1,0) = -1;
	A(1,1) = -5;

	B(0, 0) = 1;

	C(0, 1) = 0;
	C(0, 0) = 1;
	C(1, 1) = 1;
	C(1, 0) = 0;

	ColumnVector X0(2);
	X0(0) = 1;
	X0(1) = 0;

	StepSignal u;
	StateSpace state;

	state.setInitState(&X0);
	state.setInputMatrix(&B);
	state.setOutputMatrix(&C);
	state.setStateMatrix(&A);
	state.setInputSignal(&u);

	ColumnVector t(2000);
	for (int i = 0; i < t.rows(); ++i) {
		t(i) = i * 0.01;
	}

	std::cout << state.dispatch(t);

	return EXIT_SUCCESS;
}



