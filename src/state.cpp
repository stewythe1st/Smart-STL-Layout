
#include "state.h"

void state::add_projection(projection p){
	m_projections.push_back(p);
    m_x.push_back(-1);
    m_y.push_back(-1);
    m_rot.push_back(-1);
}


void state::randomize() {
	for (int i = 0; i < m_projections.size(); i++) {
		m_x[i] = rand() % m_xsize;
		m_y[i] = rand() % m_ysize;
		m_rot[i] = rand() % 360;
		m_projections[i].rotate((float)m_rot[i]);
	}
	return;
}


void state::print(std::string filename) {
	bitmap_image bmp(m_xsize, m_ysize);
	bmp.clear(0xFF);
	for (int i = 0; i < m_projections.size(); i++) {
		m_projections[i].print_on_bmp(bmp, m_x[i], m_y[i], palette_colormap[rand() % e_black]);
	}
	bmp.save_image(filename);
	return;
}


void state::calc_fitness() {

	// Variables
	projection* p;
	
	m_fitness = 0;

	// Allocate a grid
	bool** grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			grid[x][y] = false;
		}
	}

	// Mark each projection on the grid
	for (int i = 0; i < m_projections.size(); i++) {
		p = &m_projections[i];
		for (int x = 0; x < p->get_xsize(); x++) {
			for (int y = 0; y < p->get_ysize(); y++) {
				if (p->get_pixel(x, y)) {

					// If out of bounds
					if (x + m_x[i] > p->get_xsize() && y + m_y[i] > p->get_ysize() ) {
						m_fitness += OUT_OF_BOUNDS_PENALTY;
					}

					// If overlapping
					else if (grid[x + m_x[i]][y + m_y[i]]) {
						m_fitness += OVERLAP_PENALTY;
					}
				}
			}
		}
	}

	// Clean up
	for (int x = 0; x < m_xsize; x++) {
		delete[] grid[x];
	}
	delete[] grid;

	return;
}