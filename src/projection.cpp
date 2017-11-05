
#include "common.h"
#include "projection.h"
#include "cmath"


projection::projection() {
	m_xsize			= 0;
	m_ysize			= 0;
	m_grid			= nullptr;
}


projection::~projection() {
	// Delete old grids
	for (int x = 0; x < m_xsize; x++) {
		delete[] m_grid[x];
	}
	delete[] m_grid;
	return;
}


projection::projection(int xnew, int ynew) {

	// Assign new data
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


projection::projection(const projection& p) {
	// Assign new data
	m_xsize = p.m_xsize;
	m_ysize = p.m_ysize;

	// Allocate new grids
	m_grid = new bool*[m_xsize];
	for (int x = 0; x < m_xsize; x++) {
		m_grid[x] = new bool[m_ysize];
		for (int y = 0; y < m_ysize; y++) {
			m_grid[x][y] = p.m_grid[x][y];
		}
	}
	return;
}


projection& projection::operator=(const projection& rhs) {

	// Delete old grids
	for (int x = 0; x < m_xsize; x++) {
		delete[] m_grid[x];
	}
	delete[] m_grid;

	// Assign new data
	m_xsize = rhs.m_xsize;
	m_ysize = rhs.m_ysize;

	// Allocate new grids
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


bool** projection::rotate(float rot, int& xsize_out, int& ysize_out) {

	// Variables
	int size_rotated;
	int origin_rotated;
	float xorigin = (float)m_xsize / 2.0f;
	float yorigin = (float)m_ysize / 2.0f;
	float intpart;
	float xnew;
	float ynew;
	int xmax = INT_MIN;
	int xmin = INT_MAX;
	int ymax = INT_MIN;
	int ymin = INT_MAX;
	bool** grid_rotated;
	bool** grid_rotated_minimized;

	// Calculate rotated grid size
	size_rotated = 2 * MAX2(m_xsize, m_ysize);
	origin_rotated = size_rotated / 2;
	rot = fmod(rot, 360.0f);

	// Allocate rotated grid
	grid_rotated = new bool*[size_rotated];
	for (int x = 0; x < size_rotated; x++) {
		grid_rotated[x] = new bool[size_rotated];
		for (int y = 0; y < size_rotated; y++) {
			grid_rotated[x][y] = false;
		}
	}
	
	// Walk across base grid, looking for set pixels
	for (int x = 0; x < m_xsize; x++) {
		for (int y = 0; y < m_ysize; y++) {
			if (m_grid[x][y]) {

				// Translate each pixel to be centered around the origin for the rotation
				// Perform matrix operation to rotate pixel by 'rot' degrees
				//  |xnew| = | cos(rot) sin(rot)| * |x|
				//  |ynew|   |-sin(rot) cos(rot)|   |y|
				xnew = ((float)x - xorigin)*cosf(DEG_2_RAD(rot)) + ((float)y - yorigin)*sinf(DEG_2_RAD(rot)) + (float)origin_rotated;
				ynew = ((float)y - yorigin)*cosf(DEG_2_RAD(rot)) - ((float)x - xorigin)*sinf(DEG_2_RAD(rot)) + (float)origin_rotated;
				if (abs(modf(xnew, &intpart)) >= ANTIALIASING_THRESHOLD) {
					grid_rotated[(int)xnew + (intpart >= 0 ? 1 : -1)][(int)ynew] = true;
				}
				if (abs(modf(ynew, &intpart)) >= ANTIALIASING_THRESHOLD) {
					grid_rotated[(int)xnew][(int)ynew + (intpart >= 0 ? 1 : -1)] = true;
				}
				grid_rotated[(int)xnew][(int)ynew] = true;
				xmax = MAX2(xmax, (int)xnew);
				xmin = MIN2(xmin, (int)xnew);
				ymax = MAX2(ymax, (int)ynew);
				ymin = MIN2(ymin, (int)ynew);
			}
		}
	}
	
	// Assign new grid data
	xsize_out = xmax - xmin;
	ysize_out = ymax - ymin;
	grid_rotated_minimized = new bool*[xsize_out];
	for (int x = 0; x < xsize_out; x++) {
		grid_rotated_minimized[x] = new bool[ysize_out];
		for (int y = 0; y < ysize_out; y++) {
			grid_rotated_minimized[x][y] = grid_rotated[x + xmin][y + ymin];
		}
	}

	// Clean up
	for (int i = 0; i < size_rotated; i++)
		delete[] grid_rotated[i];
	delete[] grid_rotated;

	return grid_rotated_minimized;
}


void projection::print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, float rot, const rgb_t color) {
	
	// Variables
	bool** grid;
	int xsize, ysize;
	
	// Rotate
	grid = rotate(rot, xsize, ysize);

	// Draw on bitmap
	for (int x = 0; x < xsize; x++) {
		for (int y = 0; y < ysize; y++) {
			if (grid[x][y] && x + x_offset < (int)bmp.width() && y + y_offset < (int)bmp.height()) {
				bmp.set_pixel(x + x_offset, y + y_offset, color);
			}
		}
	}

	// Clean up
	for (int x = 0; x < xsize; x++)
		delete[] grid[x];
	delete[] grid;

	return;
}
