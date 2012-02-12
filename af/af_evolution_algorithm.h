/*
 * af_evolution_algorithm.h
 *
 *  Created on: Feb 12, 2012
 *      Author: kononencheg
 */

#ifndef AF_EVOLUTION_ALGORITHM_H_
#define AF_EVOLUTION_ALGORITHM_H_

#include "af_evolution_individual.h"
#include "af_evolution_population.h"

typedef af_evolution_population * (* af_evolution_init_function) ();

typedef void (* af_evolution_fitness_function)
					(af_evolution_individual * individual);

typedef af_evolution_individual * (* af_evolution_select_function)
					(af_evolution_population * population);

typedef void (* af_evolution_mutate_function)
					(af_evolution_individual * child);

typedef af_evolution_individual * (* af_evolution_crossover_function)
										(af_evolution_individual * man,
										 af_evolution_individual * woman);


typedef struct _af_evolution_algorithm {
	time_t time;

	af_evolution_population * population;

	af_evolution_init_function init_function;

	af_evolution_fitness_function fitness_function;

	af_evolution_select_function select_function;

	af_evolution_crossover_function crossover_function;

	af_evolution_mutate_function mutate_function;

	double elitism_ratio;

} af_evolution_algorithm;

af_evolution_algorithm * af_evolution_algorithm_alloc(time_t time);

void af_evolution_algorithm_set_init_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_init_function function);

void af_evolution_algorithm_set_fitness_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_fitness_function function);

void af_evolution_algorithm_set_select_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_select_function function);

void af_evolution_algorithm_set_crossover_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_crossover_function function);

void af_evolution_algorithm_set_mutate_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_mutate_function function);

af_evolution_individual * af_evolution_algorithm_process
							(af_evolution_algorithm * algorithm);

void af_evolution_algorithm_free(af_evolution_algorithm * algorithm);

#endif /* AF_EVOLUTION_ALGORITHM_H_ */
