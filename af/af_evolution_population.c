
#include "af_evolution_population.h"

af_evolution_population * af_evolution_population_alloc(size_t size) {
	af_evolution_population * population =
			(af_evolution_population *) malloc(sizeof(af_evolution_population));

	population->individuals = (af_evolution_individual **)
			malloc(size * sizeof(af_evolution_individual));

	population->size = size;

	return population;
}

void af_evolution_population_free(af_evolution_population * population) {
	size_t i = 0;

	while (i < population->size) {
		af_evolution_individual_free(population->individuals[i]);
		i++;
	}

	free(population->individuals);
	free(population);
}
