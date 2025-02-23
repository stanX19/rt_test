#include "vulkan_test.hpp"
// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan.h>

int main()
{
	std::ofstream fs("image1.ppm");
	std::cout.rdbuf(fs.rdbuf());

	hittable_list world;

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);
	auto material_glass = make_shared<dielectric>(1.5);
	auto material_bubble_outer = make_shared<dielectric>(1.33);
	auto material_bubble_inner = make_shared<dielectric>(0.75);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-0.8, 0.6, -0), 0.1, material_left));
	world.add(make_shared<sphere>(point3(-0.8, 0.9, -1), 0.1, material_left));
	world.add(make_shared<sphere>(point3(-0.8, 1.2, -2), 0.1, material_left));
	for (int i  = 0; i < 9; i++) {
		world.add(make_shared<sphere>(point3(-1.0 + (i % 3) * 0.4 + (i % 2) * 0.1, -0.1 + (i / 3) * 0.4 - (i % 2) * 0.1, 0.5 - (i / 9) * 1.0), 0.05 + (i % 3) * 0.05, material_bubble_inner));
		world.add(make_shared<sphere>(point3(-1.0 + (i % 3) * 0.4 + (i % 2) * 0.1, -0.1 + (i / 3) * 0.4 - (i % 2) * 0.1, 0.5 - (i / 9) * 1.0), 0.06 + (i % 3) * 0.05, material_bubble_outer));
	}
	world.add(make_shared<sphere>(point3(0.4, -0.3, 0.3), 0.2, material_glass));
	world.add(make_shared<sphere>(point3(-0.4, -0.3, 0.5), 0.2, material_right));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 10;
	cam.max_depth = 20;

	if (0) {
		cam.image_width = 1920;
		cam.samples_per_pixel = 1000;
		cam.max_depth = 100;
	}

	cam.render(world);
}