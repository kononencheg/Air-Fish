#==============================================================================
# Variables
#==============================================================================

CPP_FLAGS = -g3 -fno-inline -O0
LINK_FLAGS = --link-stand-alone

BUILD_DIR := bin
INCLUDE_DIRS := /usr/include/octave-3.4.0 \
				/usr/include/octave-3.4.0/octave \
				./math 
				
OBJECT_RULES = $(notdir $(wildcard $(BUILD_DIR)/*.o))

VPATH = ./math 

#==============================================================================
# Rules
#==============================================================================

all : air-fish
	
air-fish : $(OBJECT_RULES)
	mkoctfile $(LINK_FLAGS) -o $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $^)

%.o : %.cpp 
	g++ -o $(BUILD_DIR)/$@ $(addprefix -I, $(INCLUDE_DIRS)) $(CPP_FLAGS) -c  $< 

clean :
	rm -rf $(BUILD_DIR)/*.o
	