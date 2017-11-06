
#include "population.h"


state* pool::chooseParentKTourn(int k) {
	return &m_states[kTournament(k, true, true)];
}


void pool::reduceByKTourn(size_t size, int k) {

	// Sanity check
	if (m_states.size() <= size)
		return;

	// Variables
	int idx;

	// Run tournaments until we've shrunk to desired size
	while (m_states.size() > size) {

		// Since we're not allowing replacement, need to make sure k isn't bigger than size
		if (k > (int)m_states.size())
			k = (int)m_states.size();

		// Randomly pick k members of the pool, keeping track of the one with the worst fitness 
		idx = kTournament(k, true, false);
		m_states.erase(m_states.begin() + idx);
	}

	return;
}


int pool::kTournament(int k, bool highest, bool replacement) {

	// Variables
	int*	tournament = new int[k];
	int		bestIdx;
	bool	inTournament;

	// Randomly pick members for the tournament, with or without replacement
	for (int i = 0; i < k; i++) {
		tournament[i] = rand() % m_states.size();
		if (!replacement) {
			inTournament = false;
			for (int j = 0; j < i && !inTournament; j++) {
				if (tournament[j] == tournament[i]) {
					i--;
					break;
				}
			}
		}
	}

	// Run tournament, looking for best or worst fitness	
	bestIdx = tournament[0];
	for (int i = 1; i < k; i++) {
		if (highest && m_states[tournament[i]].get_fitness() > m_states[bestIdx].get_fitness()) {
			bestIdx = tournament[i];
		}
		else if (!highest && m_states[tournament[i]].get_fitness() < m_states[bestIdx].get_fitness()) {
			bestIdx = tournament[i];
		}
	}

	// Clean up
	delete[] tournament;
	return bestIdx;
}

state* pool::get_best() {
	state* best = &m_states.front();
	for (std::vector<state>::iterator it = m_states.begin(); it != m_states.end(); it++) {
		if ((*it).get_fitness() < best->get_fitness()) {
			best = &(*it);
		}
	}
	return best;
}

float pool::get_average() {
	int total = 0;
	for (std::vector<state>::iterator it = m_states.begin(); it != m_states.end(); it++) {
		total += (*it).get_fitness();
	}
	return (float)total / (float)m_states.size();
}
