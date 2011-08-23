BUILD_DIR := bin

OCTAVE_INCLUDE_DIR := /usr/include/octave-3.4.0

all : air-fish

air-fish : main.o
	mkoctfile --link-stand-alone -o $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/, $^)

main.o : main.cpp
	g++ -o $(BUILD_DIR)/$@ -I$(OCTAVE_INCLUDE_DIR) -I$(OCTAVE_INCLUDE_DIR)/octave -c -g3 -fno-inline -O0 $< 

clean :
	rm -rf $(BUILD_DIR)/*.o