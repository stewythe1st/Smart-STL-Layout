
#include "common.h"
#include "projection.h"
#include "cmath"


projection::projection() {
	m_xsize			= 0;
	m_ysize			= 0;
	m_xsize_base	= 0;
	m_ysize_base	= 0;
	m_rot			= 0;
	m_grid_base		= nullptr;
	m_grid			= nullptr;
}

projection::projection(int xnew, int ynew) {
	m_xsize = xnew;
	m_ysize = ynew;
	m_xsize_base = xnew;
	m_ysize_base = ynew;
	m_rot	= 0;
	m_grid_base = new bool*[m_xsize];
	m_grid = new bool*[m_xsize];	
	for (int x = 0; x < m_xsize; x++) {
		m_grid_base[x] = new bool[m_ysize];
		m_grid[x] = new bool[m_ysize];		
		for (int y = 0; y < m_ysize; y++) {
			m_grid_base[x][y] = false;
			m_grid[x][y] = false;
		}
	}
	return;
}


projection& projection::operator=(const projection& rhs) {
	m_xsize = rhs.m_xsize;
	m_ysize = rhs.m_ysize;
	m_xsize_base = rhs.m_xsize_base;
	m_ysize_base = rhs.m_ysize_base;
	m_rot	= rhs.m_rot;
	m_grid_base = new bool*[m_xsize];
	m_grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		m_grid_base[x] = new bool[m_ysize];
		m_grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			m_grid_base[x][y] = rhs.m_grid_base[x][y];
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


void projection::rotate(float rot) {

	// Variables
	int size_temp;
	int origin_temp;
	float xorigin_base = (float)m_xsize_base / 2.0f;
	float yorigin_base = (float)m_ysize_base / 2.0f;
	float intpart;
	float xnew;
	float ynew;
	int xnew_max = INT_MIN;
	int xnew_min = INT_MAX;
	int ynew_max = INT_MIN;
	int ynew_min = INT_MAX;
	bool** grid_temp;

	// Delete old grid
	for (int i = 0; i < m_xsize; i++)
		delete[] m_grid[i];
	delete[] m_grid;

	// Calculate temp grid size
	size_temp = 2 * MAX2(m_xsize_base, m_ysize_base);
	origin_temp = size_temp / 2;
	rot = fmod(rot + m_rot, 360.0f);

	// Allocate temp grid
	grid_temp = new bool*[size_temp];
	for (int x = 0; x < size_temp; x++) {
		grid_temp[x] = new bool[size_temp];
		for (int y = 0; y < size_temp; y++) {
			grid_temp[x][y] = false;
		}
	}
	
	// Walk across base grid, looking for set pixels
	for (int x = 0; x < m_xsize_base; x++) {
		for (int y = 0; y < m_ysize_base; y++) {
			if (m_grid_base[x][y]) {

				// Translate each pixel to be centered around the origin for the rotation
				// Perform matrix operation to rotate pixel by 'rot' degrees
				//  |xnew| = | cos(rot) sin(rot)| * |x|
				//  |ynew|   |-sin(rot) cos(rot)|   |y|
				xnew = ((float)x - xorigin_base)*cosf(DEG_2_RAD(rot)) + ((float)y - yorigin_base)*sinf(DEG_2_RAD(rot)) + (float)origin_temp;
				ynew = ((float)y - yorigin_base)*cosf(DEG_2_RAD(rot)) - ((float)x - xorigin_base)*sinf(DEG_2_RAD(rot)) + (float)origin_temp;
				if (abs(modf(xnew, &intpart)) >= ANTIALIASING_THRESHOLD) {
					grid_temp[(int)xnew + (intpart >= 0 ? 1 : -1)][(int)ynew] = true;
				}
				if (abs(modf(ynew, &intpart)) >= ANTIALIASING_THRESHOLD) {
					grid_temp[(int)xnew][(int)ynew + (intpart >= 0 ? 1 : -1)] = true;
				}
				grid_temp[(int)xnew][(int)ynew] = true;
				xnew_max = MAX2(xnew_max, (int)xnew);
				xnew_min = MIN2(xnew_min, (int)xnew);
				ynew_max = MAX2(ynew_max, (int)ynew);
				ynew_min = MIN2(ynew_min, (int)ynew);
			}
		}
	}
	
	// Assign new grid data
	m_rot = rot;
	m_xsize = xnew_max - xnew_min;
	m_ysize = ynew_max - ynew_min;
	m_grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		m_grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			m_grid[x][y] = grid_temp[x + xnew_min][y + ynew_min];
		}
	}

	// Clean up
	for (int i = 0; i < size_temp; i++)
		delete[] grid_temp[i];
	delete[] grid_temp;

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
