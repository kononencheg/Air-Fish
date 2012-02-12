#==============================================================================
# Variables
#==============================================================================

CC = clang 

CFLAGS = -g3 -fno-inline -O0
LIBS = gsl gslcblas

BUILD_DIR := bin
INCLUDE_DIRS := 
				
VPATH = af

#==============================================================================
# Rules
#==============================================================================

all : air-fish
	
air-fish : 	main.o \
			af_block_demux.o \
			af_block_gain.o \
			af_block_mux.o \
			af_block_print.o \
			af_block_state_space.o \
			af_block_step_response.o \
			af_block_step.o \
			af_block_sum.o \
			af_block.o \
			af_evolution_algorithm.o \
			af_evolution_individual.o \
			af_evolution_population.o \
			af_router.o \
			af_state_space.o \
			af_step_response.o \
				  
				  
	$(CC) $(LINK_FLAGS) -o $(BUILD_DIR)/$@ \
	   	  $(addprefix $(BUILD_DIR)/, $^) \
	   	  $(addprefix -l, $(LIBS)) 

%.o : %.c
	
	$(CC) -o $(BUILD_DIR)/$@ $(CFLAGS) -c $< 

clean :
	
	rm -rf $(BUILD_DIR)/*.*