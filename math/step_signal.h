/**
 * Air-Fish
 *
 * @file step_signal.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef STEPSIGNAL_H_
#define STEPSIGNAL_H_

#include "i_signal_dispatcher.h"

class StepSignal : public ISignalDispatcher {
	private:

		double m_threshold;

	public:

		StepSignal() : m_threshold(0.0) {}
		StepSignal(double threshold) : m_threshold(threshold) {}
		StepSignal(StepSignal &signal) : m_threshold(signal.m_threshold) {}

		virtual ~StepSignal() {}

		Matrix dispatch(const ColumnVector &timeLine);
		ColumnVector dispatch(double time);
};

#endif /* STEPSIGNAL_H_ */
