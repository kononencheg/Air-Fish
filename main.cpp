/**
 * Air-Fish
 *
 * @file main.cpp
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stdlib.h>

#include <iostream>

#include <octave/config.h>
#include <octave/Matrix.h>
#include <octave/ODEFunc.h>
#include <octave/LSODE.h>

Matrix A(2, 2);
Matrix B(2, 1);

double heaviside(double t) {
	return t >= 0 ? 1.0 : 0.0;
}

ColumnVector stateEquation(const ColumnVector &x, double t) {
	ColumnVector result(2);

	for (int i = 0; i < x.length(); i++) {
		float value = heaviside(t) * B(i, 0);

		for (int j = 0; j < A.columns(); j++) {
			value += A(i, j) * x(j);
		}

		result(i) = value;
	}

	return result;
}

int main() {
	A(0,0) = 1.0;
	A(0,1) = 10.0;
	A(1,0) = -1.0;
	A(1,1) = -5.0;

	B(0, 0) = 1.0;
	B(1, 0) = 0.0;

	ColumnVector t(100);
	for (int i = 0; i < 100; i ++) {
		t(i) = i * 0.1;
	}

	ColumnVector X0(2);
	X0(0) = 0.0;
	X0(1) = 0.0;

	Matrix X;

	ODEFunc solve(stateEquation);

	LSODE ls(X0, 0.0, solve);
	X = ls.do_integrate(t);

	std::cout << X;

	return EXIT_SUCCESS;
}



