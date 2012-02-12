/**
 * Air-Fish
 *
 * @file af_step_response.c
 * @author Kononenko Sergey <kononenheg@gmail.com>
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <gsl/gsl_math.h>

#include "af_step_response.h"

af_step_response *af_step_response_alloc(size_t size, double error_ratio) {
	af_step_response *response =
			(af_step_response *) malloc(sizeof(af_step_response));

	response->time = (double *) malloc(size * sizeof(double));
	response->value = (double *) malloc(size * sizeof(double));
	response->length = 0;

	response->error_ratio = error_ratio;
	response->steady_value = 0;

	response->last_peak_value = GSL_NEGINF;
	response->max_value = GSL_NEGINF;

	response->derivative = GSL_POSINF;

	return response;
}

void af_step_response_add_value(af_step_response * response,
								const double time, double value) {
	double derivative, last_value, last_time;
	size_t i = response->length;

	if (response->length > 1) {
		last_value = response->value[i - 1];
		last_time = response->time[i - 1];

		derivative = (value - last_value) / (time - last_time);
	}

	response->length++;

	response->time[i] = time;
	response->value[i] = value;

	if (response->max_value < value) {
		response->max_value = value;
	}

	if (derivative * response->derivative < 0) {
		response->last_peak_value = value;
		response->last_peak_length = 0;
	}

	response->steady_value =
		(response->steady_value * response->last_peak_length + value) /
				(response->last_peak_length + 1);

	response->last_peak_length++;

	response->derivative = derivative;

	if (response->last_peak_value == GSL_NEGINF) {
		response->steady_value = value;
	}
}

double af_step_response_get_steady_value(af_step_response * response) {
	return response->steady_value;
}

double af_step_response_get_overshoot(af_step_response * response) {
	return (response->max_value - response->steady_value) /
				response->steady_value;
}

double af_step_response_get_settling_time(af_step_response * response) {
	double settling_time = GSL_NEGINF;

	double error_band_radius = response->error_ratio * response->steady_value;
	double error_band_max = response->steady_value + error_band_radius;
	double error_band_min = response->steady_value - error_band_radius;

	size_t i = 0;

	if (error_band_max < response->max_value) {

		while (i < response->length) {
			if (error_band_min < response->value[i] &&
				response->value[i] < error_band_max) {
				if (settling_time == GSL_NEGINF) {
					settling_time = response->time[i];
				}
			} else {
				if (settling_time != GSL_NEGINF) {
					settling_time = GSL_NEGINF;
				}
			}

			i++;
		}
	}

	return settling_time;
}

void af_step_response_reset(af_step_response * response) {
	response->length = 0;
	response->steady_value = 0;
	response->last_peak_value = GSL_NEGINF;
	response->max_value = GSL_NEGINF;
	response->derivative = GSL_POSINF;
}

void af_step_response_free(af_step_response * response) {
	free(response->time);
	free(response->value);
	free(response);
}
