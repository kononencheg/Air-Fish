BUILD_DIR := bin

OCTAVE_INCLUDE_DIR := /usr/include/octave-3.4.0

all : air-fish

air-fish : main.o step_signal.o state_space.o
	mkoctfile --link-stand-alone -o $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $^)

main.o : main.cpp math/step_signal.h math/state_space.h
	g++ -o $(BUILD_DIR)/$@ -I$(OCTAVE_INCLUDE_DIR) -I$(OCTAVE_INCLUDE_DIR)/octave -c -g3 -fno-inline -O0 $< 

step_signal.o : math/step_signal.cpp math/step_signal.h math/i_signal_dispatcher.h
	g++ -o $(BUILD_DIR)/$@ -I$(OCTAVE_INCLUDE_DIR) -I$(OCTAVE_INCLUDE_DIR)/octave -c -g3 -fno-inline -O0 $< 

state_space.o : math/state_space.cpp math/state_space.h math/i_signal_dispatcher.h
	g++ -o $(BUILD_DIR)/$@ -I$(OCTAVE_INCLUDE_DIR) -I$(OCTAVE_INCLUDE_DIR)/octave -c -g3 -fno-inline -O0 $< 

clean :
	rm -rf $(BUILD_DIR)/*.o