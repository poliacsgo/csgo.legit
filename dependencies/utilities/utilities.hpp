#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"
#include "../../source-sdk/classes/c_usercmd.hpp"
#define M_PI 3.14159265358979323846

namespace utilities {
	namespace math {
		template <typename t> t clamp_value(t value, t min, t max) {
			if (value > max) {
				return max;
			}
			if (value < min) {
				return min;
			}
			return value;
		}
		void normalize_view(vec3_t &angle);
		float distance_point_to_line(vec3_t point, vec3_t origin, vec3_t direction);
		vec3_t angle_vector(vec3_t to_convert);
		void fix_movement(c_usercmd* cmd);
		void vector_angles(const vec3_t& forward, vec3_t &angles);
		vec3_t normalize_angle(vec3_t angle);
		vec3_t calc_angle(const vec3_t& in, vec3_t out);
		void clamp_movement(float fMaxSpeed, c_usercmd* user_cmd);
		void angle_vectors_v2(const vec3_t &angles, vec3_t *forward);
	}
	namespace game {
		void* capture_interface(const char* mod, const char* iface);
	}
	std::uint8_t* pattern_scan(void* module, const char* signature);

}