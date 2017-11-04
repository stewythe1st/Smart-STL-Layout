#include "bmp\bitmap_image.hpp"
#include "config.h"
#include "state.h"
#include "stl.h"
#include <chrono>

int main(int argc, char *argv[]) {

	// Variables
	stl m;
	projection p;
	config cfg;

	// Get configuration
	if (argc > 1) {
		cfg.read(argv[1]);
	}

	// Seed random number generator
	if (cfg.seedType == SEED_TIME_BASED) {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
		cfg.seed = (unsigned int)time.time_since_epoch().count();
	}
	srand(cfg.seed);

	// Open and convert STL
	m.open("./stl/batarang.stl");
	p = m.to_projection();
	state s(500, 500);

	// State testing
	s.add_projection(p);
	s.add_projection(p);
	s.add_projection(p);
	s.randomize();
	s.calc_fitness();
	s.print("./img/test.bmp");

	return 0;
}
