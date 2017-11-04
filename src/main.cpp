#include "bmp\bitmap_image.hpp"
#include "config.h"
#include "state.h"
#include "stl.h"

int main() {

	stl m;
	m.open("./stl/batarang.stl");
	projection p = m.to_projection();

	config cfg;

	srand(111111);

	state s(500, 500);

	s.add_projection(p);
	s.add_projection(p);
	s.add_projection(p);

	s.randomize();

	s.calc_fitness();

	s.print("./img/test.bmp");
	
	return 0;
}
