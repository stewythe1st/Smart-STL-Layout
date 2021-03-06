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
	config cfg;
	pool population;
	pool offspring;
	state* parent1;
	state* parent2;
	state* best;
	std::vector<projection*> projs;
	int eval;
	std::ofstream log;
	std::ofstream log2;
	std::chrono::high_resolution_clock::time_point start;
	long long int time_diff;
	bool terminate;
	int last_best = 0;
	int unchanged = 0;

	// Get configuration
	if (argc > 1) {
		cfg.read(argv[1]);
	}

	// Open log file
	log.open("./log/" + cfg.name + ".csv");
	log2.open("./log/" + cfg.name + "_population.csv");
	if (!log.is_open() || !log2.is_open()) {
		std::cout << "Error: Unable to write to log file" << std::endl;
		exit(1);
	}

	// Seed random number generator
	if (cfg.seedType == SEED_TIME_BASED) {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
		cfg.seed = (unsigned int)time.time_since_epoch().count();
	}
	srand(cfg.seed);

	// Construct projections
	for (size_t i = 0; i < cfg.stls.size(); i++) {
		std::cout << "Parsing STL file for " << cfg.stls[i] << "... ";
		stl* temp_stl = new stl(cfg.stls[i]);
		projection* temp_proj = temp_stl->to_projection(cfg.stl_scales[i]);
		temp_proj->calc_rotations(cfg.rotationalRestriction);
		projs.push_back(temp_proj);
		delete temp_stl;
		std::cout << "Done!" << std::endl;
	}

	state initial(&projs, cfg.width, cfg.height);

	log << "Run,Generation,Evaluations,Average Fitness,Best Fitness,Time" << std::endl;
	std::cout << std::endl << "<Gen>\t<Eval>\t<Avg>\t<Best>\t<Time(ms)>" << std::endl;
	
	// Runs
	for (int run = 0; run < cfg.runs; run++) {
		std::cout << std::endl << "Run " << run << std::endl;
		eval = 0;
		state local_best(&projs, cfg.width, cfg.height);

		// Construct and randomize initial population
		for (int i = 0; i < cfg.mu; i++) {
			population.add(initial);
			population.get(i)->randomize();
			population.get(i)->calc_fitness();
			eval++;
		}		

		// Evals
		terminate = false;
		for (int generation = 1; eval < cfg.evals && !terminate; generation++) {
			start = std::chrono::high_resolution_clock::now();

			// Generate Offspring
			for (int i = 0; i < cfg.lambda; i++) {
				state s(&projs, cfg.width, cfg.height);
				parent1 = population.chooseParentKTourn(cfg.parentSelTournSize);
				parent2 = population.chooseParentKTourn(cfg.parentSelTournSize);
				s.nPointCrossover(parent1, parent2, cfg.crossovers);
				if (GET_RANDOM_DECIMAL < cfg.mutationRate) {
					switch (cfg.mutationType) {
					case MUTATION_RANDRESET:
						s.randResetMutate();
						break;
					case MUTATION_CREEP:
						s.creepMutate(cfg.creepDist);
						break;
					}
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
			if (best->get_fitness() > local_best.get_fitness()) {
				local_best = *best;
			}

			// Print info for this gen
			time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
			std::cout << generation << "\t" << eval << "\t" << FORMAT_FLOAT(0) << population.get_average() << "\t" << best->get_fitness() << "\t" << time_diff << std::endl;
			log << run << "," << generation << "," << eval << "," << FORMAT_FLOAT(0) << population.get_average() << "," << best->get_fitness() << "," << time_diff << std::endl;

			// Termination test
			switch(cfg.termTest){
				case TERMTEST_EVALS:
					terminate = (eval >= cfg.termTarget);
					break;
				case TERMTEST_GENS:
					terminate = (generation >= cfg.termTarget);
					break;
				case TERMTEST_BESTUNCHANGED:
					unchanged = (last_best == best->get_fitness() ? unchanged + 1 : 0 );
					terminate = (unchanged >= cfg.termTarget);
					last_best = best->get_fitness();
					break;
			}
		}
		
		// Print output
		std::cout << std::endl << "Final: " << std::endl;
		for (int i = 0; i < (int)cfg.stls.size(); i++) {
			std::cout << "\t" << cfg.stls[i] << std::string(32 - cfg.stls[i].size(), ' ') << local_best.get_position_string(i) << std::endl;
		}
		std::string output_filename = "./img/" + cfg.name + "_run_" + std::to_string(run) + ".bmp";
		std::cout << std::endl << "Printing output to: " << output_filename << " ..." << std::endl;
		local_best.print(output_filename);

		population.log(run, log2);

		population.empty();
	}
	
	// Clean up
	for (std::vector<projection*>::iterator it = projs.begin(); it != projs.end(); it++) {
		delete *it;
	}
	log.close();
	log2.close();
	
	return 0;
}
