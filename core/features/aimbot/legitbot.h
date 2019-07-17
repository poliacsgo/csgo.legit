#pragma once
#include "../../../dependencies/common_includes.hpp"
class aimbot : public singleton<aimbot>
{
public:
	//initalizers
	void initalize(c_usercmd* user_cmd);

	//features
	void run_aimbot(player_t* local, c_usercmd* user_cmd);

	vec3_t calc_angle(const vec3_t& vecSource, const vec3_t& vecDestination)
	{
		vec3_t qAngles;
		vec3_t delta = vec3_t((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
		float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / M_PI));
		qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / M_PI));
		qAngles[2] = 0.f;
		if (delta[0] >= 0.f)
			qAngles[1] += 180.f;

		return qAngles;
	}
	vec3_t normalize(vec3_t angle) {
		if (angle.x > 89.f)
			angle.x = 89.f;

		if (angle.x < -89.f)
			angle.x = -89.f;

		if (angle.y > 180)
			angle.y = 180;

		if (angle.y < -180)
			angle.y = -180;

		angle.z = 0;

		return angle;
	}
};

