/**
 * Air-Fish
 *
 * @file state_space.cpp
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include "state_space.h"

// Use semaphore in future
StateSpace *StateSpace::s_currentInstance = NULL;

/* =========================================================================
	Public methods
   ========================================================================= */

ColumnVector StateSpace::dispatch(double time) {
	s_currentInstance = this;
	ColumnVector result = (*m_outputMatrix) * m_stateSolve->do_integrate(time);
	s_currentInstance = NULL;

	return result;
}

Matrix StateSpace::dispatch(const ColumnVector &timeLine) {
	s_currentInstance = this;
	Matrix result = (*m_outputMatrix) * m_stateSolve->do_integrate(timeLine).transpose();
	s_currentInstance = NULL;

	return result;
}

/* =========================================================================
	Private methods
   ========================================================================= */

ColumnVector StateSpace::processEquation(const ColumnVector &stateVector, double time) {
	return (*m_stateMatrix) * stateVector + (*m_inputMatrix) * m_inputSignal->dispatch(time);
}
