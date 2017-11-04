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
	state local_best(500, 500);
	state global_best(500, 500);

	// Runs
	for (int run = 0; run < cfg.runs; run++) {
		std::cout << std::endl << "Run " << run + 1 << std::endl;

		// Evals
		for (int eval = 0; eval < cfg.evals; eval++) {

			// Generate random state
			state* s = new state(500, 500);
			s->add_projection(p);
			s->add_projection(p);
			s->add_projection(p);
			s->randomize();
			s->calc_fitness();

			// Keep track of local best fitness
			if (eval == 0 || s->get_fitness() < local_best.get_fitness()) {
				local_best = *s;
				std::cout << eval << "\t" << local_best.get_fitness() << std::endl;
			}

			delete s;
		}

		// Keep track of local best fitness
		if (local_best.get_fitness() < global_best.get_fitness()) {
			global_best = local_best;
		}
	}
	
	// Print global best
	global_best.print("./img/default.bmp");

	return 0;
}
