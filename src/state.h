#ifndef STATE_H
#define STATE_H


#include "bmp/bitmap_image.hpp"
#include "projection.h"
#include <string>


class state {
private:
	std::vector<projection>		m_projections;
	std::vector<int>			m_x;
	std::vector<int>			m_y;
	std::vector<int>			m_rot;
	
	int							m_xsize = 500;
	int							m_ysize = 500;
	
public:

	// Constructors & Destructors
	state() {};
	state(int x, int y) { m_xsize = x; m_ysize = y; };
	~state() {};

	// Member Functions
	void add_projection(projection p);
	void randomize();
	void print(std::string filename);

};


#endif
