#ifndef STATE_H
#define STATE_H


#include "bmp/bitmap_image.hpp"
#include "projection.h"
#include <climits>
#include <string>

#define OVERLAP_PENALTY			1
#define OUT_OF_BOUNDS_PENALTY	2
#define COMPRESSION_BONUS		2


class state {
private:
	std::vector<projection*>*	m_states;
	std::vector<int>			m_x;
	std::vector<int>			m_y;
	std::vector<int>			m_rot;
	
	int							m_xsize;
	int							m_ysize;

	int							m_fitness = INT_MIN;
	
public:

	// Constructors & Destructors
	state() {};
	state(std::vector<projection*>* p, int x, int y);
	~state() {};
	state& operator=(const state& rhs);

	// Member Functions
	void randomize();
	void print(std::string filename);
	std::string get_position_string(int i);
	void calc_fitness();
	void nPointCrossover(state* parent1, state* parent2, int n);
	void randResetMutate();
	void creepMutate(int creepDist);

	// Accessors and Mutators
	int get_fitness() { return m_fitness; };

};


#endif
