#include "../common_includes.hpp"

void c_math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

void c_math::transform_vector(vec3_t& a, matrix_t& b, vec3_t& out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

void c_math::vector_angles(vec3_t& forward, vec3_t& angles) {
	if (forward.y == 0.0f && forward.x == 0.0f) {
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles.y = 0.0f; //yaw left/right
	}
	else {
		angles.x = atan2(-forward.z, vec2_t(forward).length()) * -180 / static_cast<float>(pi);
		angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(pi);

		if (angles.y > 90)
			angles.y -= 180;
		else if (angles.y < 90)
			angles.y += 180;
		else if (angles.y == 90)
			angles.y = 0;
	}

	angles.z = 0.0f;
}

void c_math::angle_vectors(vec3_t& angles, vec3_t& forward) {
	float sp, sy, cp, cy;

	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.x), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void c_math::angle_vectors(vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(deg_to_rad(angles.x), &sp, &cp);
	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

vec3_t c_math::calculate_angle(vec3_t& a, vec3_t& b) {
	vec3_t angles;

	vec3_t delta = a - b;

	vector_angles(delta, angles);

	return angles;
}

vec3_t c_math::vector_add(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t c_math::vector_subtract(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t c_math::vector_multiply(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t c_math::vector_divide(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool c_math::screen_transform(const vec3_t& point, vec3_t& screen) {

	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

bool c_math::world_to_screen(const vec3_t &origin, vec3_t &screen) {

	auto matrix = interfaces::engine->world_to_screen_matrix();

	auto find_point = [](vec3_t &point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
		point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
	};

	float w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = (float)((screen_width / 2) + (0.5 * ((matrix[0][0] * origin.x
		+ matrix[0][1] * origin.y
		+ matrix[0][2] * origin.z
		+ matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen.y = (float)((screen_height / 2) - (0.5 * ((matrix[1][0] * origin.x
		+ matrix[1][1] * origin.y
		+ matrix[1][2] * origin.z
		+ matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
		find_point(screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
}

float c_math::distance_to_ray(vec3_t &pos, vec3_t &ray_start, vec3_t &ray_end, float *along, vec3_t *point_on_ray) {

	vec3_t to = pos - ray_start;
	vec3_t dir = ray_end - ray_start;
	float length = dir.normalize_float();

	float range_along = dir.dot(to);
	if (along)
		*along = range_along;

	float range;

	if (range_along < 0.0f)
	{
		range = -(pos - ray_start).length();

		if (point_on_ray)
			*point_on_ray = ray_start;
	}
	else if (range_along > length)
	{
		range = -(pos - ray_end).length();

		if (point_on_ray)
			*point_on_ray = ray_end;
	}
	else
	{
		vec3_t on_ray = ray_start + range_along * dir;
		range = (pos - on_ray).length();

		if (point_on_ray)
			*point_on_ray = on_ray;
	}

	return range;

}