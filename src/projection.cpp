
#include "common.h"
#include "projection.h"
#include "cmath"
#include <climits>


projection::projection() {
	m_color = palette_colormap[rand() % e_black];
}


projection::projection(int xsize, int ysize) {
	layout l;
	l.m_xsize = xsize;
	l.m_ysize = ysize;
	l.m_grid = new bool*[xsize];
	for (int x = 0; x < xsize; x++) {
		l.m_grid[x] = new bool[ysize];
		for (int y = 0; y < ysize; y++) {
			l.m_grid[x][y] = false;
		}
	}
	m_rotations.push_back(l);
	m_color = palette_colormap[rand() % e_black];
}


projection::~projection() {
	for (std::vector<layout>::iterator it = m_rotations.begin(); it != m_rotations.end(); it++) {
		for (int x = 0; x < (*it).m_xsize; x++) {
			delete[] (*it).m_grid[x];
		}
		delete[] (*it).m_grid;
	}
	m_rotations.clear();
	return;
}

projection& projection::operator=(const projection& rhs) {
	m_color = rhs.m_color;
	for (std::vector<layout>::const_iterator it = rhs.m_rotations.begin(); it != rhs.m_rotations.end(); it++) {
		layout l;
		l.m_xsize = (*it).m_xsize;
		l.m_ysize = (*it).m_ysize;
		l.m_grid = new bool*[l.m_xsize];
		for (int x = 0; x < l.m_xsize; x++) {
			for (int y = 0; y < l.m_ysize; y++) {
				l.m_grid = (*it).m_grid;
			}
		}
		m_rotations.push_back(l);
	}
	return *this;
}


void projection::calc_rotations(int rot_restrict) {
	layout l;
	for (int rot = rot_restrict; rot < 360; rot+=rot_restrict) {
		l.m_grid = rotate((float)rot, l.m_xsize, l.m_ysize);
		m_rotations.push_back(l);
	}
	return;
}


bool** projection::rotate(float rot, int& xsize_out, int& ysize_out) {

	// Variables
	layout base = m_rotations.front();
	int size_rotated;
	int origin_rotated;
	float xorigin = (float)base.m_xsize / 2.0f;
	float yorigin = (float)base.m_ysize / 2.0f;
	double intpart;
	float xnew;
	float ynew;
	int xmax = INT_MIN;
	int xmin = INT_MAX;
	int ymax = INT_MIN;
	int ymin = INT_MAX;
	bool** grid_rotated;
	bool** grid_rotated_minimized;

	// Calculate rotated grid size
	size_rotated = 2 * MAX2(base.m_xsize, base.m_ysize);
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
	for (int x = 0; x < base.m_xsize; x++) {
		for (int y = 0; y < base.m_ysize; y++) {
			if (base.m_grid[x][y]) {

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


void projection::print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, float rot) {
	
	// Variables
	bool** grid;
	int xsize, ysize;
	
	// Rotate
	grid = rotate(rot, xsize, ysize);

	// Draw on bitmap
	for (int x = 0; x < xsize; x++) {
		for (int y = 0; y < ysize; y++) {
			if (grid[x][y] && x + x_offset < (int)bmp.width() && y + y_offset < (int)bmp.height()) {
				bmp.set_pixel(x + x_offset, y + y_offset, m_color);
			}
		}
	}

	// Clean up
	for (int x = 0; x < xsize; x++)
		delete[] grid[x];
	delete[] grid;

	return;
}
