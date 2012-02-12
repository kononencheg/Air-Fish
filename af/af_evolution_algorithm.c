
#include <time.h>

#include <gsl/gsl_math.h>

#include "af_evolution_algorithm.h"

void algorithm_iteration(af_evolution_algorithm  * algorithm,
						 af_evolution_population * parents,
						 af_evolution_individual ** best, time_t start_time);

af_evolution_algorithm * af_evolution_algorithm_alloc(time_t time) {
	af_evolution_algorithm * algorithm =
			(af_evolution_algorithm *) malloc(sizeof(af_evolution_algorithm));

	algorithm->time = time;

	return algorithm;
}

void af_evolution_algorithm_set_init_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_init_function function) {

	algorithm->init_function = function;
}

void af_evolution_algorithm_set_fitness_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_fitness_function function) {

	algorithm->fitness_function = function;
}

void af_evolution_algorithm_set_select_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_select_function function) {

	algorithm->select_function = function;
}

void af_evolution_algorithm_set_crossover_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_crossover_function function) {

	algorithm->crossover_function = function;
}

void af_evolution_algorithm_set_mutate_function
		(af_evolution_algorithm * algorithm,
				const af_evolution_mutate_function function) {

	algorithm->mutate_function = function;
}

af_evolution_individual * af_evolution_algorithm_process
							(af_evolution_algorithm * algorithm) {

	af_evolution_individual * best = NULL;

	algorithm_iteration
		(algorithm, algorithm->init_function(), &best, time(NULL));

	return best;
}

void af_evolution_algorithm_free(af_evolution_algorithm * algorithm) {
	free(algorithm);
}

void algorithm_iteration(af_evolution_algorithm  * algorithm,
						 af_evolution_population * parents,
						 af_evolution_individual ** best, time_t start_time) {

	af_evolution_population * children = NULL;
	af_evolution_individual * current = NULL;
	size_t i = 0;

	while (i < parents->size) {
		current = parents->individuals[i];

		algorithm->fitness_function(current);

		if ((*best) == NULL || current->fitness > (*best)->fitness) {
			(*best) = current;
		}

		i++;
	}

	if (time(NULL) - start_time < algorithm->time) {
		children = af_evolution_population_alloc(parents->size);

		i = 0;
		while (i < children->size) {
			children->individuals[i] = algorithm->crossover_function(
				algorithm->select_function(parents),
				algorithm->select_function(parents)
			);

			algorithm->mutate_function(children->individuals[i]);

			i++;
		}

		af_evolution_population_free(parents);

		algorithm_iteration(algorithm, children, best, start_time);
	}
}
