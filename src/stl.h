#ifndef STL_H
#define STL_H


#include "bmp/bitmap_image.hpp"
#include "projection.h"
#include <string>


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

	// Private member functions
	void open_binary(std::ifstream& in);
	void open_ascii(std::ifstream& in);
	
public:

	// Constructors & Destructors
	stl();
	inline stl(std::string filename) { open(filename); };
	~stl();

	// Member Functions
	void open(std::string filename);
	projection* to_projection(float scale);
};


#endif
