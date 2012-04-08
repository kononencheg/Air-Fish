
#include <gsl/gsl_math.h>
#include "af_evolution_individual.h"

af_evolution_individual * af_evolution_individual_alloc
				(double * const genotype, const size_t genotype_size) {

	af_evolution_individual * individual =
			(af_evolution_individual *) malloc(sizeof(af_evolution_individual));

	individual->genotype = genotype;
	individual->genotype_size = genotype_size;
	individual->fitness = GSL_NEGINF;

	return individual;
}

af_evolution_individual * af_evolution_individual_clone
							(af_evolution_individual * individual) {
	af_evolution_individual * new_individual;
	double * genotype = (double *)
			malloc(individual->genotype_size * sizeof(double));

	size_t i = 0;
	while (i < individual->genotype_size) {
		genotype[i] = individual->genotype[i];

		i++;
	}

	new_individual =
			af_evolution_individual_alloc(genotype, individual->genotype_size);

	new_individual->fitness = individual->fitness;

	return new_individual;
}

void af_evolution_individual_free(af_evolution_individual * individual) {
	free(individual->genotype);
	free(individual);
}
