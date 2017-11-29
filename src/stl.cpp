// STL file format documentation openly available at
// https://en.wikipedia.org/wiki/STL_(file_format)


#include "common.h"
#include "stl.h"
#include <climits>
#include <fstream>
#include <iostream>


stl::stl() {
	m_header[0] = '\0';
	m_numTriangles = 0;
	m_triangles = nullptr;
}


stl::~stl() {
	delete[] m_triangles;
	return;
}


void stl::open(std::string filename) {

	// Variables
	std::ifstream	in;
	char			buf[7];

	in.open(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cout << "Error: Unable to read file" << std::endl;
		exit(1);
	}

	// Read first 6 chars
	in.getline(buf, 7);
	in.clear();
	in.seekg(0);

	if (!strcmp(buf, "solid ")) {
		open_ascii(in);
	}
	else {
		open_binary(in);
	}

	in.close();
	return;
}


void stl::open_binary(std::ifstream& in) {

	// Variables
	char			buf[4];

	// Read in header
	in.read(m_header, sizeof(m_header) / sizeof(*m_header));

	// Read in number of triangles
	in.read(buf, sizeof(m_numTriangles));
	memcpy((void*)&m_numTriangles, buf, sizeof(m_numTriangles));

	// Allocat triangle memory
	m_triangles = new triangle[m_numTriangles];

	// Read in each triangle
	for (uint32_t i = 0; i < m_numTriangles; i++) {

		// Normal vector
		for (int j = 0; j < 3; j++) {
			in.read(buf, sizeof(m_triangles[i].normal[j]));
			memcpy((void*)&m_triangles[i].normal, buf, sizeof(m_triangles[i].normal));
		}
		// Vertex 1
		for (int j = 0; j < 3; j++) {
			in.read(buf, sizeof(m_triangles[i].vertex1[j]));
			memcpy((void*)&m_triangles[i].vertex1[j], buf, sizeof(m_triangles[i].vertex1));
		}
		// Vertex 2
		for (int j = 0; j < 3; j++) {
			in.read(buf, sizeof(m_triangles[i].vertex2[j]));
			memcpy((void*)&m_triangles[i].vertex2[j], buf, sizeof(m_triangles[i].vertex2[j]));
		}
		// Vertex 3
		for (int j = 0; j < 3; j++) {
			in.read(buf, sizeof(m_triangles[i].vertex3[j]));
			memcpy((void*)&m_triangles[i].vertex3[j], buf, sizeof(m_triangles[i].vertex3[j]));
		}
		// Attribute Byte Count
		in.read(buf, sizeof(m_triangles[i].attribByteCnt));
		memcpy((void*)&m_triangles[i].attribByteCnt, buf, sizeof(m_triangles[i].attribByteCnt));
	}

	return;
}

void stl::open_ascii(std::ifstream& in) {

	// Variables
	char			buf[64];
	long int		lines;

	// Counts lines and allocate triangles
	lines = (long int)std::count(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), '\n') + 1;
	in.clear();
	in.seekg(0);
	m_numTriangles = (lines - 3) / 7;
	m_triangles = new triangle[m_numTriangles];

	// Read in header
	in.getline(buf, 64);
	memcpy(m_header, buf + strlen("solid "), strlen(buf) + 1);

	for (uint32_t i = 0; i<m_numTriangles; i++) {

		// "facet normal ni nj nk"
		in.getline(buf, 64);
		for (int j = 0; j < 3; j++) {
			memcpy(buf, buf + 15, strlen(buf) + 1);
			m_triangles[i].normal[j] = (float)atof(buf);
		}

		// "outer loop" - discard
		in.getline(buf, 64);

		// "vertex v1x v1y v1z"
		in.getline(buf, 64);
		for (int j = 0; j < 3; j++) {
			memcpy(buf, buf + 14, strlen(buf) + 1);
			m_triangles[i].vertex1[j] = (float)atof(buf);
		}

		// "vertex v2x v2y v2z"
		in.getline(buf, 64);
		for (int j = 0; j < 3; j++) {
			memcpy(buf, buf + 14, strlen(buf) + 1);
			m_triangles[i].vertex2[j] = (float)atof(buf);
		}

		// "vertex v3x v3y v3z"
		in.getline(buf, 64);
		for (int j = 0; j < 3; j++) {
			memcpy(buf, buf + 14, strlen(buf) + 1);
			m_triangles[i].vertex3[j] = (float)atof(buf);
		}

		// "endloop" - discard
		in.getline(buf, 64);

		// "endfacet" - discard
		in.getline(buf, 64);
	}

	return;
}


// Adapted from http://forum.devmaster.net/t/advanced-rasterization/6145
projection* stl::to_projection(float scale) {

	// Variables
	int x1;
	int x2;
	int x3;
	int y1;
	int y2;
	int y3;
	int minx;
	int maxx;
	int miny;
	int maxy;
	int global_minx = INT_MAX;
	int global_maxx = INT_MIN;
	int global_miny = INT_MAX;
	int global_maxy = INT_MIN;

	// Find global min/max
	for (uint32_t i = 0; i < m_numTriangles; i++) {
		x1 = (int)(m_triangles[i].vertex1[0] * scale);
		x2 = (int)(m_triangles[i].vertex2[0] * scale);
		x3 = (int)(m_triangles[i].vertex3[0] * scale);
		y1 = (int)(m_triangles[i].vertex1[1] * scale);
		y2 = (int)(m_triangles[i].vertex2[1] * scale);
		y3 = (int)(m_triangles[i].vertex3[1] * scale);
		global_maxx = MAX4(global_maxx, x1, x2, x3);
		global_minx = MIN4(global_minx, x1, x2, x3);
		global_maxy = MAX4(global_maxy, y1, y2, y3);
		global_miny = MIN4(global_miny, y1, y2, y3);
	}


	// Make new projection
	projection* rtn = new projection(global_maxx - global_minx, global_maxy - global_miny);

	// Loop through all the triangles
	for (uint32_t i = 0; i < m_numTriangles; i++) {

		// Get coords
		x1 = (int)(m_triangles[i].vertex1[0] * scale) - global_minx;
		x2 = (int)(m_triangles[i].vertex2[0] * scale) - global_minx;
		x3 = (int)(m_triangles[i].vertex3[0] * scale) - global_minx;
		y1 = (int)(m_triangles[i].vertex1[1] * scale) - global_miny;
		y2 = (int)(m_triangles[i].vertex2[1] * scale) - global_miny;
		y3 = (int)(m_triangles[i].vertex3[1] * scale) - global_miny;

		// Get bounding rectangle
		minx = MIN3(x1, x2, x3);
		maxx = MAX3(x1, x2, x3);
		miny = MIN3(y1, y2, y3);
		maxy = MAX3(y1, y2, y3);

		// Scan through bounding rectangle
		for (int y = miny; y < maxy; y++) {
			for (int x = minx; x < maxx; x++) {
				if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) >= 0 &&
					(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) >= 0 &&
					(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) >= 0) {
					rtn->set_pixel(x, y);
				}

			}
		}
	}

	return rtn;
}
