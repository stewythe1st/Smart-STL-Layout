
#include "state.h"


state::state(std::vector<projection>* p, int x, int y) {
	m_states = p;
	m_xsize = x;
	m_ysize = y;
	for (size_t i = 0; i < p->size(); i++) {
		m_x.push_back(0);
		m_y.push_back(0);
		m_rot.push_back(0);
	}
}


state& state::operator=(const state& rhs) {
	m_states = rhs.m_states;
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_rot = rhs.m_rot;
	m_xsize = rhs.m_xsize;
	m_ysize = rhs.m_ysize;
	m_fitness = rhs.m_fitness;
	return *this;
}


void state::randomize() {
	for (size_t i = 0; i < m_states->size(); i++) {
		m_x[i] = rand() % m_xsize;
		m_y[i] = rand() % m_ysize;
		m_rot[i] = rand() % 360;
	}
	return;
}


void state::print(std::string filename) {
	bitmap_image bmp(m_xsize, m_ysize);
	bmp.clear(0xFF);
	for (size_t i = 0; i < m_states->size(); i++) {
		(*m_states)[i].print_on_bmp(bmp, m_x[i], m_y[i], (float)m_rot[i]);
	}
	bmp.save_image(filename);
	return;
}


void state::calc_fitness() {

	// Variables
	projection* p;
	bool** grid;
	bool** proj_grid;
	int xsize, ysize;
	bool valid;
	
	m_fitness = 0;

	// Allocate a grid
	grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			grid[x][y] = false;
		}
	}

	// Mark each projection on the grid
	for (size_t i = 0; i < m_states->size(); i++) {
		p = &(*m_states)[i];

		proj_grid = p->rotate((float)m_rot[i], xsize, ysize);

		for (int x = 0; x < xsize; x++) {
			for (int y = 0; y < ysize; y++) {

				if (proj_grid[x][y]) {
					valid = true;

					// If out of bounds
					if (x + m_x[i] >= m_xsize || y + m_y[i] >= m_ysize ) {
						m_fitness += OUT_OF_BOUNDS_PENALTY;
					}

					// If overlapping
					else if (grid[x + m_x[i]][y + m_y[i]]) {
						m_fitness += OVERLAP_PENALTY;
					}

					// Mark grid
					else {
						grid[x + m_x[i]][y + m_y[i]] = true;
					}
				}
			}
		}
		// Reset grid
		for (int x = 0; x < xsize; x++) {
			delete[] proj_grid[x];
		}
		delete[] proj_grid;
	}

	// Clean up
	for (int x = 0; x < m_xsize; x++) {
		delete[] grid[x];
	}
	delete[] grid;

	return;
}


void state::nPointCrossover(state* parent1, state* parent2, int n) {

	// Variables
	int*	crossoverPts;
	int		size = (int)m_states->size();

	// Sanity check
	if (n > size) {
		n = size;
	}

	// Generate swap points
	crossoverPts = new int[n];
	for (int i = 0; i < n; i++) {
		crossoverPts[i] = rand() % size;
	}
	std::sort(crossoverPts, crossoverPts + n);

	// Walk down chromosome and copy genes into offspring
	int j = 0;
	for (int i = 0; i < size; i++) {

		// Every time we hit a crossover point, swap parents
		if (i >= crossoverPts[j] && j < n) {
			std::swap(parent1, parent2);
			j++;
		}
		m_x[i] = parent1->m_x[i];
		m_y[i] = parent1->m_y[i];
		m_rot[i] = parent1->m_rot[i];
	}

	// Clean up
	delete[] crossoverPts;

	return;
}


void state::creepMutate(int creepDist) {

	// Pick random values
	int idx = rand() % m_states->size();
	int attrib = rand() % 3;
	int creep = rand() % (2 * creepDist) - creepDist;

	// Apply creep
	switch (attrib) {
	case 0:
		m_x[idx] += creepDist;
		break;
	case 1:
		m_y[idx] += creepDist;
		break;
	case 2:
		m_rot[idx] = (m_rot[idx] + creepDist) % 360;
		break;
	}

	return;
}


void state::randResetMutate() {

	// Variables
	int idx;

	// Pick a random gene
	idx = rand() % m_states->size();

	// Choose new random coordinates
	m_x[idx] = rand() % m_xsize;
	m_y[idx] = rand() % m_ysize;
	m_rot[idx] = rand() % 360;

	return;
}
