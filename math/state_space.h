/**
 * Air-Fish
 *
 * @file state_space.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef STATE_SPACE_H_
#define STATE_SPACE_H_

#include <iostream>

#include <octave/config.h>
#include <octave/ODEFunc.h>
#include <octave/LSODE.h>

#include "i_signal_dispatcher.h"

class StateSpace: public ISignalDispatcher {
	private:

		ISignalDispatcher *m_inputSignal;

		Matrix *m_stateMatrix;

		Matrix *m_inputMatrix;

		Matrix *m_outputMatrix;

		ODEFunc *m_stateFunc;

		LSODE *m_stateSolve;

		/* =================================================================
			Private methods
		   ================================================================= */

		ColumnVector processEquation(const ColumnVector &stateVector, double time);


		/* =================================================================
			Private static objects
		   ================================================================= */

		static ColumnVector equationWrapper(const ColumnVector &stateVector, double time) {
			return s_currentInstance->processEquation(stateVector, time);
		}

		static StateSpace *s_currentInstance;

	public:

		/* =================================================================
			Constructor and destructor
		   ================================================================= */

		StateSpace() : m_stateSolve(NULL) {
			m_stateFunc = new ODEFunc(equationWrapper);
		}

		virtual ~StateSpace() {
			delete m_stateFunc;
		}

		/* =================================================================
			Public methods
		   ================================================================= */

		Matrix dispatch(const ColumnVector &timeLine);
		ColumnVector dispatch(double time);

		/* =================================================================
			Getters and setters
		   ================================================================= */

		inline void setInputSignal(ISignalDispatcher *signal) {
			m_inputSignal = signal;
		}

		inline void setStateMatrix(Matrix *stateMatrix) {
			m_stateMatrix = stateMatrix;
		}

		inline void setInputMatrix(Matrix *inputMatrix) {
			m_inputMatrix = inputMatrix;
		}

		inline void setOutputMatrix(Matrix *outputMatrix) {
			m_outputMatrix = outputMatrix;
		}

		void setInitState(ColumnVector *stateVector) {
			if (m_stateSolve != NULL) {
				delete m_stateSolve;
			}

			m_stateSolve = new LSODE(*stateVector, 0.0, *m_stateFunc);
		}
};

#endif /* STATE_SPACE_H_ */
