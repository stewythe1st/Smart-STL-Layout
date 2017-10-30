#ifndef PROJECTION_H
#define PROJECTION_H

#include "bmp\bitmap_image.hpp"

#define ANTIALIASING_THRESHOLD	0.50f


// Origin is at top-left. x is left-to-right, y is top-to-bottom
class projection {

private:
	int			m_xsize;
	int			m_ysize;
	bool**		m_grid;

	float		m_rot;

	int			m_xsize_base;
	int			m_ysize_base;
	bool**		m_grid_base;

public:

	// Constructors & Destructors
	projection();
	~projection();
	projection(int x, int y);
	projection(const projection& p);
	projection& projection::operator=(const projection& rhs);

	// Member Functions
	void draw_bounding_box();
	void rotate(float deg);
	void print_on_bmp(bitmap_image& bmp, int x_offset, int y_offset, const rgb_t color);

	// Accessors & mutators
	inline void set_pixel(int x, int y) { m_grid[x][y] = true; };
	inline void set_origin_pixel(int x, int y) { m_grid_base[x][y] = m_grid[x][y] = true; };
	inline void clear_pixel(int x, int y) { m_grid[x][y] = false; };
	inline bool get_pixel(int x, int y) const { return m_grid[x][y]; };

};



#endif
