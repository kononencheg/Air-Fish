
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include "af/af_block.h"
#include "af/af_block_sum.h"
#include "af/af_block_mux.h"
#include "af/af_block_step.h"
#include "af/af_block_gain.h"
#include "af/af_block_print.h"
#include "af/af_block_demux.h"
#include "af/af_block_state_space.h"
#include "af/af_block_step_response.h"

#include "af/af_evolution_algorithm.h"
#include "af/af_evolution_individual.h"
#include "af/af_evolution_population.h"


#include "af/af_router.h"

#include "af/af_state_space.h"
#include "af/af_step_response.h"

#define POPULATION_SIZE (1000)

#define SIMULATION_TIME_STEP (0.5)
#define SIMULATION_TIME (20)

#define EVOLUTION_TIME (5*60)

void fill_router();

af_evolution_population * init_population();

void access_fitness(af_evolution_individual * individual);
af_evolution_individual * select_parent(af_evolution_population * parents);
af_evolution_individual * crossover(af_evolution_individual * man,
									af_evolution_individual * woman);

void mutate(af_evolution_individual * individual);

gsl_rng * rnd_generator;

af_router * router;

af_state_space * state_space;

af_block * control_gain_block;
af_block * print_block;

af_step_response *response;

int main() {
	af_evolution_individual * best;

	gsl_rng_env_setup();

	rnd_generator = gsl_rng_alloc(gsl_rng_taus);

	gsl_rng_set(rnd_generator, time(NULL));

	fill_router();

	af_evolution_algorithm * algorithm =
			af_evolution_algorithm_alloc(EVOLUTION_TIME);

	af_evolution_algorithm_set_init_function(algorithm, init_population);
	af_evolution_algorithm_set_fitness_function(algorithm, access_fitness);
	af_evolution_algorithm_set_select_function(algorithm, select_parent);
	af_evolution_algorithm_set_crossover_function(algorithm, crossover);
	af_evolution_algorithm_set_mutate_function(algorithm, mutate);
	
	
	best = af_evolution_algorithm_process(algorithm);


	af_step_response_reset(response);
	af_state_space_reset(state_space);

	af_block_set_params(control_gain_block, best->genotype);
	af_router_add_block(router, print_block);
	af_router_process(router, SIMULATION_TIME);

	best->fitness = (1 / af_step_response_get_settling_time(response));

	printf("F: %f\n", best->fitness);
	printf("T: %f\n", af_step_response_get_settling_time(response));
	printf("K: [ %f, %f, %f, %f ]\n", best->genotype[0], best->genotype[1], best->genotype[2], best->genotype[3]);

	return EXIT_SUCCESS;
}

void mutate(af_evolution_individual * individual) {
	double gene;
	size_t i = 0;

	while (i < individual->genotype_size) {
		gene = individual->genotype[i] *
				(gsl_rng_uniform(rnd_generator) * 0.2 + 0.9);

		if (gene <= 100 && gene >= -100) {
			individual->genotype[i] = gene;
		}

		i++;
	}
}

af_evolution_individual * crossover(af_evolution_individual * man,
									af_evolution_individual * woman) {

	double * genotype = (double *) malloc(man->genotype_size * sizeof(double));

	size_t i = 0;
	while (i < man->genotype_size) {
		if (gsl_rng_uniform(rnd_generator) < 0.5) {
			genotype[i] = man->genotype[i];
		} else {
			genotype[i] = woman->genotype[i];
		}

		i++;
	}

	return af_evolution_individual_alloc(genotype, man->genotype_size);
}

af_evolution_individual * select_parent(af_evolution_population * parents) {
	af_evolution_individual * best = NULL;
	double * range_list = (double *) malloc(parents->size * sizeof(double));
	double range = 0, selection_range;
	size_t i = 0, size = parents->size;

	while (i < parents->size) {
		range_list[i] = range;
		range += parents->individuals[i]->fitness *
				 parents->individuals[i]->fitness;
		i++;
	}

	selection_range = (range * gsl_rng_uniform(rnd_generator));

	i = 0, size = parents->size - 1;
	while (i < size) {
		if (range_list[i + 1] > selection_range) {
			best =  parents->individuals[i];
			break;
		}

		i++;
	}

	if (best == NULL) {
		best =  parents->individuals[i];
	}

	free(range_list);

	return best;
}

void access_fitness(af_evolution_individual * individual) {
	af_block_set_params(control_gain_block, individual->genotype);
	af_router_process(router, SIMULATION_TIME);

	individual->fitness = (0.001) +
			(1 / af_step_response_get_settling_time(response));

	af_step_response_reset(response);
	af_state_space_reset(state_space);
	
	/*printf("F: %f\n", individual->fitness);
	printf("K: [ %f, %f, %f, %f ]\n",
		   individual->genotype[0],
		   individual->genotype[1],
		   individual->genotype[2],
		   individual->genotype[3]);*/

	/*printf("F: %f\t", individual->fitness);
	printf("K: [ %f, %f ]\n",
			individual->genotype[0],
			individual->genotype[1]
	);*/
}

af_evolution_population * init_population() {
	af_evolution_population * population =
			af_evolution_population_alloc(POPULATION_SIZE);

	double * genotype;
	size_t i, j;

	i = 0;
	while (i < population->size) {
		genotype = (double *) malloc(4 * sizeof(double));

		j = 0;
		while (j < 4) {
			genotype[j] = gsl_rng_uniform(rnd_generator) * 200 - 100;
			j++;
		}

		population->individuals[i] =
				af_evolution_individual_alloc(genotype, 4);

		i++;
	}

	return population;
}

void fill_router() {

	const double A[] = { 0,  0,  1,  0,
						 0,  0,  0,  1,
						 2, -1,  0,  0,
						-2,  2,  0,  0 };

	const double B[] = { 0,
						 0,
						 1,
						 0 };

	const double C[] = { 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1 };

	double X0[] = { 0, 0, 0, 0 };
	double K[] = { 0, 0, 0, 0 };

	response =
			af_step_response_alloc(SIMULATION_TIME/SIMULATION_TIME_STEP, 0.05);

	router = af_router_alloc(SIMULATION_TIME_STEP);

	state_space = af_state_space_alloc(4, 1, 4);

	af_state_space_set_state_matrix(state_space, A);
	af_state_space_set_input_matrix(state_space, B);
	af_state_space_set_output_matrix(state_space, C);
	af_state_space_set_state_vector(state_space, X0);
	
	
	
	print_block 	   = af_block_print_alloc(router);
	control_gain_block = af_block_gain_alloc(router, 4, K);
	
	af_block * step_block = af_block_step_alloc(router, 0, 1);
	
	af_block * state_space_block = af_block_state_space_alloc(router, state_space);
	
    af_block * inverse_block 	= af_block_inverse_alloc(router, 4);	
	af_block * response_block 	= af_block_step_response_alloc(router, response);

	af_block * demux_block_y 	= af_block_demux_alloc(router, 0, 1);
	af_block * demux_block_x 	= af_block_demux_alloc(router, 1, 3);

	af_block * input_sum_block = af_block_sum_alloc(router, 2);
	
	af_block * control_mux_block  = af_block_mux_alloc(router, 2, 4);
	
    af_block * control_demux_block_x1 = af_block_demux_alloc(router, 0, 1);
	af_block * control_demux_block_x2 = af_block_demux_alloc(router, 1, 1);
	af_block * control_demux_block_x3 = af_block_demux_alloc(router, 2, 1);
	af_block * control_demux_block_x4 = af_block_demux_alloc(router, 3, 1);
	
	af_block * control_sum_block 	 = af_block_sum_alloc(router, 4);
	
	
    af_block_add_source_at(print_block, state_space_block, 0);
	
        //af_block_add_source_at(state_space_block, step_block, 0);

	af_block_add_source_at(response_block, state_space_block, 0);
	af_block_add_source_at(inverse_block, state_space_block, 0);
	
	af_block_add_source_at(demux_block_y, inverse_block, 0);
	af_block_add_source_at(demux_block_x, inverse_block, 0);
		
	af_block_add_source_at(input_sum_block, demux_block_y, 0);
	af_block_add_source_at(input_sum_block, step_block, 1);
	
	af_block_add_source_at(control_mux_block, input_sum_block, 0);
	af_block_add_source_at(control_mux_block, demux_block_x, 1);	
	
    af_block_add_source_at(control_gain_block, control_mux_block, 0);

	af_block_add_source_at(control_demux_block_x1, control_gain_block, 0);
	af_block_add_source_at(control_demux_block_x2, control_gain_block, 0);	
	af_block_add_source_at(control_demux_block_x3, control_gain_block, 0);	
	af_block_add_source_at(control_demux_block_x4, control_gain_block, 0);

	af_block_add_source_at(control_sum_block, control_demux_block_x1, 0);
	af_block_add_source_at(control_sum_block, control_demux_block_x2, 1);
	af_block_add_source_at(control_sum_block, control_demux_block_x3, 2);
	af_block_add_source_at(control_sum_block, control_demux_block_x4, 3);
	
    af_block_add_source_at(state_space_block, control_sum_block, 0);
	
	
	af_router_add_block(router, step_block);
	af_router_add_block(router, state_space_block);
	
    af_router_add_block(router, inverse_block);
	af_router_add_block(router, response_block);

	af_router_add_block(router, demux_block_y);
	af_router_add_block(router, demux_block_x);
	
	af_router_add_block(router, input_sum_block);
	
	af_router_add_block(router, control_gain_block);

	af_router_add_block(router, control_mux_block);
	
	af_router_add_block(router, control_demux_block_x1);
	af_router_add_block(router, control_demux_block_x2);
	af_router_add_block(router, control_demux_block_x3);
	af_router_add_block(router, control_demux_block_x4);
	
	af_router_add_block(router, control_sum_block);
	
}
