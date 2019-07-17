#include "bunnyhop.hpp"


template<class T, class U>
inline T clamp_value(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void bhop::bunnyhop(c_usercmd* user_cmd) {
	player_t* local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());
	if (user_cmd->buttons & in_jump) {
		if (!(local->flags() & fl_onground))
			user_cmd->buttons &= ~in_jump;
	}
}

static vec_t normalize_yaw(vec_t ang)
{
	while (ang < -180.0f)
		ang += 360.0f;
	while (ang > 180.0f)
		ang -= 360.0f;
	return ang;
}

void bhop::autostrafe(c_usercmd* user_cmd) {
	player_t* local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());
	if (!local) return;

	if (!(local->flags() & fl_onground)) {
		float cl_sidespeed = interfaces::console->get_convar("cl_sidespeed")->GetFloat();
		if (GetAsyncKeyState('S')) {
			user_cmd->viewangles.y -= 180;
		}
		else if (GetAsyncKeyState('D')) {
			user_cmd->viewangles.y -= 90;
		}
		else if (GetAsyncKeyState('A')) {
			user_cmd->viewangles.y += 90;
		}
		if (!local->get_velocity().length_2d() > 0.5 || local->get_velocity().length_2d() == NAN || local->get_velocity().length_2d() == INFINITE)
		{
			user_cmd->forwardmove = 400;
			return;
		}

		user_cmd->forwardmove = clamp_value(5850.f / local->get_velocity().length_2d(), -400, 400);
		if ((user_cmd->forwardmove < -400 || user_cmd->forwardmove > 400))
			user_cmd->forwardmove = 0;

		const auto vel = local->get_velocity();
		const float y_vel = RAD2DEG(atan2(vel.y, vel.x));
		const float diff_ang = normalize_yaw(user_cmd->viewangles.y - y_vel);

		user_cmd->sidemove = (diff_ang > 0.0) ? -cl_sidespeed : cl_sidespeed;
		user_cmd->viewangles.y = normalize_yaw(user_cmd->viewangles.y - diff_ang);
	}

}