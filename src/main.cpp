#include "bmp\bitmap_image.hpp"
#include "stl.h"

int main() {

	stl test_stl;
	test_stl.open("./stl/batarang.stl");

	bitmap_image bmp(600, 400);
	bmp.clear(0xFF);

	projection test_proj = test_stl.to_projection();
	test_proj.print_on_bmp(bmp, 100, 100, jet_colormap[2]);

	bmp.save_image("./img/test.bmp");

	return 0;
}