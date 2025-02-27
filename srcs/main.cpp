#include "vulkan_test.hpp"
// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan.h>

int main()
{
	std::ofstream fs("image1.ppm");
	std::cout.rdbuf(fs.rdbuf());

	HittableList world;

	auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    // for (int a = -11; a < 11; a++) {
    //     for (int b = -11; b < 11; b++) {
    //         auto choose_mat = random_double();
    //         point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

    //         if ((center - point3(4, 0.2, 0)).length() > 0.9) {
    //             shared_ptr<Material> sphere_material;

    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = Color::random() * Color::random();
    //                 sphere_material = make_shared<Lambertian>(albedo);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = Color::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = make_shared<Metal>(albedo, fuzz);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 sphere_material = make_shared<Dielectric>(1.5, 0.25);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             }
    //         }
    //     }
    // }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));
	
	// auto material_bubble_outer = make_shared<dielectric>(1.33);
	// auto material_bubble_inner = make_shared<dielectric>(0.75);
	// for (int i  = 0; i < 9; i++) {
	// 	world.add(make_shared<Sphere>(point3(-1.0 + (i % 3) * 0.4 + (i % 2) * 0.1, -0.1 + (i / 3) * 0.4 - (i % 2) * 0.1, 0.5 - (i / 9) * 1.0), 0.05 + (i % 3) * 0.05, material_bubble_inner));
	// 	world.add(make_shared<Sphere>(point3(-1.0 + (i % 3) * 0.4 + (i % 2) * 0.1, -0.1 + (i / 3) * 0.4 - (i % 2) * 0.1, 0.5 - (i / 9) * 1.0), 0.06 + (i % 3) * 0.05, material_bubble_outer));
	// }

	Camara cam;

	cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = Vec3(0,1,0);

    cam.defocus_angle = 0.0;
    cam.focus_dist    = 10.0;

	if (0) {
		cam.image_width = 1920;
		cam.samples_per_pixel = 1000;
		cam.max_depth = 100;
	}

	cam.render(world);
}