/**
 * Air-Fish
 *
 * @file af_step_response.h
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#ifndef AF_RESPONSE_STEP_H_
#define AF_RESPONSE_STEP_H_

#include <stddef.h>

typedef struct _af_step_response {
    double error_ratio;

    double *time;
    double *value;

    double max_value;

    double steady_value;
    double last_peak_value;
    double last_peak_length;

    double derivative;

    size_t length;

} af_step_response;

af_step_response *af_step_response_alloc(size_t size, double error_ratio);

void af_step_response_add_value(af_step_response * response,
								const double time, double value);

double af_step_response_get_steady_value(af_step_response * response);

double af_step_response_get_overshoot(af_step_response * response);

double af_step_response_get_settling_time(af_step_response * response);

void af_step_response_reset(af_step_response * response);

void af_step_response_free(af_step_response * response);


#endif /* AF_RESPONSE_STEP_H_ */
