
#include "projection.h"


projection::projection() {
	m_xsize = 0;
	m_ysize = 0;
	m_grid = nullptr;
}

projection::projection(int xnew, int ynew) {
	m_xsize = xnew;
	m_ysize = ynew;
	m_grid = new bool*[m_xsize];
	for (unsigned int x = 0; x < m_xsize; x++) {
		m_grid[x] = new bool[m_ysize];
		for (unsigned int y = 0; y < m_ysize; y++) {
			m_grid[x][y] = false;
		}
	}
	return;
}


projection& projection::operator=(const projection& rhs) {
	m_xsize = rhs.m_xsize;
	m_ysize = rhs.m_ysize;
	m_grid = new bool*[m_xsize];
	for (unsigned int x = 0; x < m_xsize; x++) {
		m_grid[x] = new bool[m_ysize];
		for (unsigned int y = 0; y < m_ysize; y++) {
			m_grid[x][y] = rhs.m_grid[x][y];
		}
	}
	return *this;
}


void projection::print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, const rgb_t color) {
	for (unsigned int x = 0; x < m_xsize; x++) {
		for (unsigned int y = 0; y < m_ysize; y++) {
			if (m_grid[x][y]) {
				bmp.set_pixel(x + x_offset, y + y_offset, color);
			}
		}
	}
	return;
}
