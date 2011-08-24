/**
 * Air-Fish
 *
 * @file i_signal_dispatcher.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef I_SIGNAL_DISPATCHER_H_
#define I_SIGNAL_DISPATCHER_H_

#include <octave/config.h>
#include <octave/Matrix.h>

class ISignalDispatcher {
	public:
		virtual Matrix dispatch(const ColumnVector &timeLine) = 0;
		virtual ColumnVector dispatch(double time) = 0;
};

#endif /* I_SIGNAL_DISPATCHER_H_ */
