#include "bmp\bitmap_image.hpp"
#include "config.h"
#include "stl.h"

int main() {

	stl test_stl;
	test_stl.open("./stl/batarang.stl");

	bitmap_image bmp(600, 400);
	bmp.clear(0xFF);

	config cfg;

	projection test_proj = test_stl.to_projection();
	test_proj.print_on_bmp(bmp, 50, 100, palette_colormap[e_blue]);

	test_proj.rotate(90.0f);
	test_proj.print_on_bmp(bmp, 225, 100, palette_colormap[e_green]);

	test_proj.rotate(45.0f);
	test_proj.draw_bounding_box();
	test_proj.print_on_bmp(bmp, 350, 100, palette_colormap[e_red]);

	bmp.save_image("./img/test.bmp");
	
	return 0;
}