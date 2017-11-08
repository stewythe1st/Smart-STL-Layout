#include "bmp/bitmap_image.hpp"
#include "config.h"
#include "common.h"
#include "population.h"
#include "state.h"
#include "stl.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[]) {

	// Variables
	stl batarang_stl;
	projection* batarang_proj;
	config cfg;
	pool population;
	pool offspring;
	state* parent1;
	state* parent2;
	state* best;
	std::vector<projection*> projs;
	int eval;
	std::ofstream log;
	std::chrono::high_resolution_clock::time_point start;
	long long int time_diff;

	// Get configuration
	if (argc > 1) {
		cfg.read(argv[1]);
	}

	// Open log file
	log.open("./log/" + cfg.name + ".csv");
	if (!log.is_open()) {
		std::cout << "Error: Unable to write to log file" << std::endl;
		exit(1);
	}
	log << "Run,Generation,Evaluations,Average Fitness,Best Fitness,Time" << std::endl;

	// Seed random number generator
	if (cfg.seedType == SEED_TIME_BASED) {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
		cfg.seed = (unsigned int)time.time_since_epoch().count();
	}
	srand(cfg.seed);

	// Construct projection
	batarang_stl.open("./stl/batarang.stl");
	batarang_proj = batarang_stl.to_projection();
	batarang_proj->calc_rotations();
	
	// Construct projection reference array
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);
	projs.push_back(batarang_proj);

	state initial(&projs, 500, 500);
	
	// Runs
	for (int run = 0; run < cfg.runs; run++) {
		std::cout << std::endl << "Run " << run + 1 << std::endl;
		eval = 0;
		state local_best(&projs, 500, 500);

		// Construct and randomize initial population
		for (int i = 0; i < cfg.mu; i++) {
			population.add(initial);
			population.get(i)->randomize();
			population.get(i)->calc_fitness();
			eval++;
		}		

		// Evals
		for (int generation = 1; eval < cfg.evals; generation++) {
			start = std::chrono::high_resolution_clock::now();

			// Generate Offspring
			for (int i = 0; i < cfg.lambda; i++) {
				state s(&projs, 500, 500);
				parent1 = population.chooseParentKTourn(cfg.parentSelTournSize);
				parent2 = population.chooseParentKTourn(cfg.parentSelTournSize);
				s.nPointCrossover(parent1, parent2, cfg.crossovers);
				if (GET_RANDOM_DECIMAL < cfg.mutationRate) {
					//s.randResetMutate();
					s.creepMutate(cfg.creepDist);
				}
				s.calc_fitness();
				offspring.add(s);
				eval++;
			}

			// Add offspring to population
			for (int i = 0; i < cfg.lambda; i++) {
				population.add(*offspring.get(i));
			}
			offspring.empty();

			// Reduce population
			population.reduceByKTourn(cfg.mu, cfg.survivorSelTournSize);

			// Keep track of local best fitness
			best = population.get_best();
			if (best->get_fitness() < local_best.get_fitness()) {
				local_best = *best;
			}

			// Print info for this gen
			time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
			std::cout << generation << "\t" << eval << "\t" << FORMAT_FLOAT(0) << population.get_average() << "\t" << best->get_fitness() << "\t" << time_diff << std::endl;
			log << run << "," << generation << "," << eval << "," << FORMAT_FLOAT(0) << population.get_average() << "," << best->get_fitness() << "\t" << time_diff << std::endl;

			// Termination test
			if (local_best.get_fitness() == 0) {
				break;
			}
		}
		
		// Print output
		local_best.print("./img/" + cfg.name + "_run_" + std::to_string(run) + ".bmp");
	}
	
	// Clean up
	delete batarang_proj;
	log.close();
	
	return 0;
}
