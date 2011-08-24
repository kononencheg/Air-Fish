/**
 * Air-Fish
 *
 * @file step_signal.cpp
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "step_signal.h"

Matrix StepSignal::dispatch(const ColumnVector &timeLine) {
	Matrix result(1, timeLine.rows(), 1);

	for (int i = 0; i < timeLine.rows(); i++) {
		if (timeLine(i) >= m_threshold) {
			result.fill(0, 0, 0, 0, i);
			break;
		}
	}

	return result;
}

ColumnVector StepSignal::dispatch(double time) {
	ColumnVector result(1);

	result(0) = time >= m_threshold ? 1 : 0;

	return result;
}
