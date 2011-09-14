#==============================================================================
# Variables
#==============================================================================

CFLAGS = -g3 -fno-inline -O0
LIBS = gsl gslcblas

BUILD_DIR := bin
INCLUDE_DIRS := 
				
VPATH = af

#==============================================================================
# Rules
#==============================================================================

all : air-fish
	
air-fish : main.o af_state_space.o \
				  af_source_signals.o \
				  af_signal_processor.o \
				  af_signal_router.o
	gcc $(LINK_FLAGS) -o $(BUILD_DIR)/$@ \
		$(addprefix $(BUILD_DIR)/, $^) \
		$(addprefix -l, $(LIBS)) 

%.o : %.c 
	gcc -o $(BUILD_DIR)/$@ $(CFLAGS) -c $< 

clean :
	rm -rf $(BUILD_DIR)/*.o
	