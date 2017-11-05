#ifndef POPULATION_H
#define POPULATION_H


#include "state.h"
#include <vector>


class population {

private:

	std::vector<state>	m_projections;
	
public:

	// Constructors & Destructors
	population() {};
	~population() {};

	// Member Functions


	// Accessors and Mutators
	inline void add(state s) { m_projections.push_back(s); };

};


#endif
