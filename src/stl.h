#ifndef STL_H
#define STL_H


#include "bmp\bitmap_image.hpp"
#include "projection.h"
#include <string>


#define MAX2(a, b)			(std::max(a, b))
#define MIN2(a, b)			(std::min(a, b))
#define MAX3(a, b, c)		(MAX2(a, MAX2(b, c)))
#define MIN3(a, b, c)		(MIN2(a, MIN2(b, c)))
#define MAX4(a, b, c, d)	(MAX2(MAX2(a, b), MAX2(c, d)))
#define MIN4(a, b, c, d)	(MIN2(MIN2(a, b), MIN2(c, d)))


struct triangle {
	float		normal[3];
	float		vertex1[3];
	float		vertex2[3];
	float		vertex3[3];
	uint16_t	attribByteCnt;
};


class stl {
private:
	char		m_header[80];
	uint32_t	m_numTriangles;
	triangle*	m_triangles;
	
public:

	// Constructors & Destructors
	stl();
	inline stl(std::string filename) { open(filename); };
	~stl();

	// Member Functions
	void open(std::string filename);
	projection to_projection();
};


#endif
