
#include "common.h"
#include "projection.h"
#include "cmath"


projection::projection() {
	m_xsize = 0;
	m_ysize = 0;
	m_grid = nullptr;
}

projection::projection(int xnew, int ynew) {
	m_xsize = xnew;
	m_ysize = ynew;
	m_grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		m_grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			m_grid[x][y] = false;
		}
	}
	return;
}


projection& projection::operator=(const projection& rhs) {
	m_xsize = rhs.m_xsize;
	m_ysize = rhs.m_ysize;
	m_grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		m_grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			m_grid[x][y] = rhs.m_grid[x][y];
		}
	}
	return *this;
}


void projection::draw_bounding_box() {
	for (int i = 0; i < m_xsize; i++)
		set_pixel(i, 0);
	for (int i = 0; i < m_ysize; i++)
		set_pixel(0, i);
	for (int i = 0; i < m_xsize; i++)
		set_pixel(i, m_ysize - 1);
	for (int i = 0; i < m_ysize; i++)
		set_pixel(m_xsize - 1, i);
	return;
}


void projection::rotate(float deg) {

	// Variables
	int xsize_new;
	int ysize_new;
	int originx_new;
	int originy_new;
	int originx = m_xsize / 2;
	int originy = m_ysize / 2;
	float intpart;
	float xnew;
	float ynew;
	bool** m_grid_new;

	// Pre-calculate new grid size
	float coordx1 = (float)(m_xsize - originx)*cosf(DEG_2_RAD(deg)) + (float)(m_ysize - originy)*sinf(DEG_2_RAD(deg)) + originx;
	float coordy1 = (float)(m_ysize - originy)*cosf(DEG_2_RAD(deg)) - (float)(m_xsize - originx)*sinf(DEG_2_RAD(deg)) + originy;
	float coordx2 = (float)(0 - originx)*cosf(DEG_2_RAD(deg)) + (float)(m_ysize - originy)*sinf(DEG_2_RAD(deg)) + originx;
	float coordy2 = (float)(m_ysize - originy)*cosf(DEG_2_RAD(deg)) - (float)(0 - originx)*sinf(DEG_2_RAD(deg)) + originy;
	float coordx3 = (float)(m_xsize - originx)*cosf(DEG_2_RAD(deg)) + (float)(0 - originy)*sinf(DEG_2_RAD(deg)) + originx;
	float coordy3 = (float)(0 - originy)*cosf(DEG_2_RAD(deg)) - (float)(m_xsize - originx)*sinf(DEG_2_RAD(deg)) + originy;
	float coordx4 = (float)(0 - originx)*cosf(DEG_2_RAD(deg)) + (float)(0 - originy)*sinf(DEG_2_RAD(deg)) + originx;
	float coordy4 = (float)(0 - originy)*cosf(DEG_2_RAD(deg)) - (float)(0 - originx)*sinf(DEG_2_RAD(deg)) + originy;
	xsize_new = (int)(MAX4(coordx1, coordx2, coordx3, coordx4) - MIN4(coordx1, coordx2, coordx3, coordx4));
	originx_new = xsize_new / 2;
	ysize_new = (int)(MAX4(coordy1, coordy2, coordy3, coordy4) - MIN4(coordy1, coordy2, coordy3, coordy4));
	originy_new = ysize_new / 2;

	// Allocate new grid
	m_grid_new = new bool*[xsize_new];
	for (int x = 0; x < xsize_new; x++) {
		m_grid_new[x] = new bool[ysize_new];
		for (int y = 0; y < ysize_new; y++) {
			m_grid_new[x][y] = false;
		}
	}
	
	// Walk across grid, looking for set pixels
	for (int x = 0; x < (int)m_xsize; x++) {
		for (int y = 0; y < (int)m_ysize; y++) {
			if (m_grid[x][y]) {

				// Perform matrix operator to transform each pixel by t deg rotation
				//  |xnew| = | cos(t) sin(t)| * |x|
				//  |ynew|   |-sin(t) cos(t)|   |y|
				xnew = (float)(x - originx)*cosf(DEG_2_RAD(deg)) + (float)(y - originy)*sinf(DEG_2_RAD(deg)) + originx_new;
				ynew = (float)(y - originy)*cosf(DEG_2_RAD(deg)) - (float)(x - originx)*sinf(DEG_2_RAD(deg)) + originy_new;
				if (abs(modf(xnew, &intpart)) >= ANTIALIASING_THRESHOLD) {
					m_grid_new[(int)xnew + (intpart >= 0 ? 1 : -1)][(int)ynew] = true;
				}
				if (abs(modf(ynew, &intpart)) >= ANTIALIASING_THRESHOLD) {
					m_grid_new[(int)xnew][(int)ynew + (intpart >= 0 ? 1 : -1)] = true;
				}
				m_grid_new[(int)xnew][(int)ynew] = true;
			}
		}
	}
	
	// Delete old grid and shallow copy new grid over
	for (int i = 0; i < m_xsize; i++)
		delete[] m_grid[i];
	delete[] m_grid;
	m_grid = m_grid_new;
	m_xsize = xsize_new;
	m_ysize = ysize_new;

	return;
}


void projection::print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, const rgb_t color) {
	for (int x = 0; x < m_xsize; x++) {
		for (int y = 0; y < m_ysize; y++) {
			if (m_grid[x][y]) {
				bmp.set_pixel(x + x_offset, y + y_offset, color);
			}
		}
	}
	return;
}
