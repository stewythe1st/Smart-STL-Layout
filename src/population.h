#ifndef POPULATION_H
#define POPULATION_H


#include "state.h"
#include <vector>


class pool {

private:

	std::vector<state>	m_states;

	// Private member functions
	int kTournament(int k, bool best, bool replacement);
	
public:

	// Constructors & Destructors
	pool() {};
	~pool() {};

	// Member Functions
	state* chooseParentKTourn(int k);
	void reduceByKTourn(size_t size, int k);
	inline void empty() { m_states.clear(); };
	state* get_best();
	float get_average();
	void log(int run, std::ofstream& out);

	// Accessors and Mutators
	inline void add(state s) { m_states.push_back(s); };
	inline state* get(int i) { return &m_states[i]; };

};


#endif
