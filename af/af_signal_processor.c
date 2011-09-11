/**
 * Air-Fish
 *
 * @file af_signal_processor.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

af_signal_processor *af_signal_processor_alloc(size_t input_dim,
											   size_t output_dim,
											   af_signal_handler handler) {

	af_signal_processor *signal_processor =
			(af_signal_processor *) malloc(sizeof(af_signal_processor));

	signal_processor->input_dim = input_dim;
	signal_processor->output_dim = output_dim;
	signal_processor->handler = handler;

	return signal_processor;
}



