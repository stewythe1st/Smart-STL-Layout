#ifndef PROJECTION_H
#define PROJECTION_H

#include"bmp\bitmap_image.hpp"

class projection {

private:
	unsigned int	m_xsize;
	unsigned int	m_ysize;
	bool**			m_grid;
public:

	// Constructors & Destructors
	projection();
	projection(int x, int y);
	projection& projection::operator=(const projection& rhs);

	// Member Functions
	void print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, const rgb_t color);

	// Accessors & mutators
	inline void set_pixel(int x, int y) { m_grid[x][y] = true; };
	inline void clear_pixel(int x, int y) { m_grid[x][y] = false; };
	inline bool get_pixel(int x, int y) const { return m_grid[x][y]; };

};



#endif