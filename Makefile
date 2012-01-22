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
	
air-fish : main.o af_input_step.o \
				  af_signal_block.o \
				  af_signal_router.o \
				  af_state_space.o \
				  af_system_state_space.o \
				  
	$(CC) $(LINK_FLAGS) -o $(BUILD_DIR)/$@ \
	   	  $(addprefix $(BUILD_DIR)/, $^) \
	   	  $(addprefix -l, $(LIBS)) 

%.o : %.c 
	$(CC) -o $(BUILD_DIR)/$@ $(CFLAGS) -c $< 

clean :
	rm -rf $(BUILD_DIR)/*.o
	