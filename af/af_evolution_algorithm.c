
#include <time.h>

#include <gsl/gsl_math.h>

#include "af_evolution_algorithm.h"

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

	time_t start_time = time(NULL);

	af_evolution_individual * best = NULL;
	af_evolution_individual * current = NULL;
	af_evolution_population * children = NULL;
	af_evolution_population * parents = algorithm->init_function();

	size_t i;

	while (time(NULL) - start_time < algorithm->time) {
		i = 0;
		while (i < parents->size) {
			current = parents->individuals[i];

			algorithm->fitness_function(current);
			if (best == NULL || current->fitness > best->fitness) {

				if (best != NULL) {
					af_evolution_individual_free(best);
				}

				best = af_evolution_individual_clone(current);

				printf("BEST: %f\t[ %f, %f ]\n",
					best->fitness,
					best->genotype[0],
					best->genotype[1]
				);
			}

			i++;
		}

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

		parents = children;
	}

	af_evolution_population_free(children);

	return best;
}

void af_evolution_algorithm_free(af_evolution_algorithm * algorithm) {
	free(algorithm);
}
