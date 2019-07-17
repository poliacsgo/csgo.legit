#pragma once
constexpr double pi = 3.14159265358979323846;

#define deg_to_rad( x )  ( (float)(x) * (float)( pi / 180.f ) )
#define rad_to_deg( x )  ( (float)(x) * (float)( 180.f / pi ) )

class c_math : public singleton< c_math > {
public:
	void sin_cos(float r, float* s, float* c);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t calculate_angle(vec3_t&, vec3_t&);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t &origin, vec3_t &screen);
	float distance_to_ray(vec3_t &pos, vec3_t &ray_start, vec3_t &ray_end, float *along = nullptr, vec3_t *point_on_ray = nullptr);
};