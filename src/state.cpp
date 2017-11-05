
#include "state.h"


state::state(std::vector<projection>* p, int x, int y) {
	m_projections = p;
	m_xsize = x;
	m_ysize = y;
	for (int i = 0; i < p->size(); i++) {
		m_x.push_back(0);
		m_y.push_back(0);
		m_rot.push_back(0);
	}
}


state& state::operator=(const state& rhs) {
	m_projections = rhs.m_projections;
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_rot = rhs.m_rot;
	m_xsize = rhs.m_xsize;
	m_ysize = rhs.m_ysize;
	m_fitness = rhs.m_fitness;
	return *this;
}


void state::randomize() {
	for (int i = 0; i < m_projections->size(); i++) {
		m_x[i] = rand() % m_xsize;
		m_y[i] = rand() % m_ysize;
		m_rot[i] = rand() % 360;
	}
	return;
}


void state::print(std::string filename) {
	bitmap_image bmp(m_xsize, m_ysize);
	bmp.clear(0xFF);
	for (int i = 0; i < m_projections->size(); i++) {
		(*m_projections)[i].print_on_bmp(bmp, m_x[i], m_y[i], (float)m_rot[i], palette_colormap[rand() % e_black]);
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
	for (int i = 0; i < m_projections->size(); i++) {
		p = &(*m_projections)[i];

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
	return;
}