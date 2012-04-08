/*
 * af_evolution_individual.h
 *
 *  Created on: Feb 12, 2012
 *      Author: kononencheg
 */

#ifndef AF_EVOLUTION_INDIVIDUAL_H_
#define AF_EVOLUTION_INDIVIDUAL_H_

#include <gsl/gsl_rng.h>

typedef struct _af_evolution_individual {
    double * genotype;

    size_t genotype_size;

    double fitness;

} af_evolution_individual;

af_evolution_individual * af_evolution_individual_alloc
						(double * const genotype, const size_t genotype_size);

af_evolution_individual * af_evolution_individual_clone
							(af_evolution_individual * individual);

void af_evolution_individual_free(af_evolution_individual * individual);

#endif /* AF_EVOLUTION_INDIVIDUAL_H_ */
