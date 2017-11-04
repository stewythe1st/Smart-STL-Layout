
#include "state.h"

void state::add_projection(projection p){
	m_projections.push_back(p);
    m_x.push_back(-1);
    m_y.push_back(-1);
    m_rot.push_back(-1);
}

void state::randomize() {
	
	// Variables
	/*int x;
	int y;
	int rot;
	int count;*/

	// Walk down shape array choosing and verifying random layouts
	for (int i = 0; i < m_projections.size(); i++) {
		//count = 0;
		//do {
			m_x[i] = rand() % m_xsize;
			m_y[i] = rand() % m_ysize;
			m_rot[i] = rand() % 360;
			m_projections[i].rotate((float)m_rot[i]);
			//count++;
		//} while (!placementIsValid(i, x, y, rot) && count < RANDOM_MAX_TRIES);

		// If no valid placement found within max tries, restart layout
		/*if (count >= RANDOM_MAX_TRIES) {
			for (int i = 0; i < m_width; i++) {
				for (int j = 0; j < m_length; j++)
					m_layout[i][j] = false;
			}
			i = 0;
		}

		// Otherwise placement is valid and we can assign
		else
			placeShape(i, x, y, rot);
			*/
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