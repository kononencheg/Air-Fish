#ifndef AF_EVOLUTION_POPULATION_H_
#define AF_EVOLUTION_POPULATION_H_

#include "af_evolution_individual.h"

typedef struct _af_evolution_population {

	struct _af_evolution_individual ** individuals;

	size_t size;

} af_evolution_population;

af_evolution_population * af_evolution_population_alloc(size_t size);

void af_evolution_population_free(af_evolution_population * population);

#endif /* AF_EVOLUTION_POPULATION_H_ */
