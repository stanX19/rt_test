#include "camera.hpp"

void Camera::render(const Hittable &world) {
	initialize();
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	ThreadPool pool(num_threads);
	std::vector<std::vector<Color>> image(image_height, std::vector<Color>(image_width));
	
	std::vector<std::future<void>> futures;
	
	std::mutex mutex;
	int total_pixels = image_width * image_height;
	int pixels_remaining = total_pixels;
	
	for (int j = 0; j < image_height; j++) {
		for (int i = 0; i < image_width; i++) {
			futures.push_back(pool.enqueue([&, i, j] {
				Color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					Ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				image[j][i] = pixel_samples_scale * pixel_color;

				if (--pixels_remaining % image_width == 0) {
					std::lock_guard<std::mutex> lock(mutex);
					std::clog << "\rLines remaining: " << pixels_remaining / image_width << ' ' << std::flush;
				}
			}));
		}
	}
	
	for (auto &f : futures) f.get();
	
	for (int j = 0; j < image_height; j++) {
		for (int i = 0; i < image_width; i++) {
			write_color(std::cout, image[j][i]);
		}
	}
	
	std::clog << "\rDone.                 \n";
}

void Camera::initialize() {
	image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	pixel_samples_scale = 1.0 / samples_per_pixel;
	center = lookfrom;

	auto theta = degrees_to_radians(vfov);
	auto h = std::tan(theta / 2);
	auto viewport_height = 2 * h * focus_dist;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	Vec3 viewport_u = viewport_width * u;
	Vec3 viewport_v = viewport_height * -v;

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;
}

Ray Camera::get_ray(int i, int j) const {
	auto offset = sample_square();
	auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
	auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;
	return Ray(ray_origin, ray_direction);
}

Vec3 Camera::sample_square() const {
	return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

Point3 Camera::defocus_disk_sample() const {
	auto p = random_in_unit_disk();
	return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

Color Camera::ray_color(const Ray &r, int depth, const Hittable &world) const {
	if (depth <= 0) return Color(0, 0, 0);
	HitRecord rec;
	if (world.hit(r, interval(1e-8, infinity), rec)) {
		Ray scattered;
		Color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, depth - 1, world);
		}
		return Color(0, 0, 0);
	}
	Vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}