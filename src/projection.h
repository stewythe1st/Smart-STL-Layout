#ifndef PROJECTION_H
#define PROJECTION_H

#include "bmp/bitmap_image.hpp"

#define ANTIALIASING_THRESHOLD	0.50f

struct layout {
	int					m_xsize;
	int					m_ysize;
	bool**				m_grid;
};


// Origin is at top-left. x is left-to-right, y is top-to-bottom
class projection {

private:

	std::vector<layout>	m_rotations;
	rgb_t				m_color;

public:

	// Constructors & Destructors
	projection();
	projection(int x, int y);
	~projection();
	projection& operator=(const projection& rhs);

	// Member Functions
	void calc_rotations();
	bool** rotate(float rot, int& xsize_out, int& ysize_out);
	void print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, float rot);

	// Accessors & mutators
	inline void set_pixel(int x, int y) { m_rotations.front().m_grid[x][y] = true; };
	inline void clear_pixel(int x, int y) { m_rotations.front().m_grid[x][y] = false; };
	inline bool get_pixel(int x, int y) const { return m_rotations.front().m_grid[x][y]; };
	inline int get_xsize() { return m_rotations.front().m_xsize; };
	inline int get_ysize() { return m_rotations.front().m_ysize; };
	inline layout get_layout(int rot) { return m_rotations[rot]; };
};



#endif
