#include "lagcomp.hpp"
#include "../../menu/menu.h"
matrix3x4_t bone_matrix[128];
void lagcomp::handle(player_t* local, c_usercmd* user_cmd) {
	int best_target = -1;
	float best_fov = 90.f;

	if (local->health() <= 0)
		return;

	for (int i = 0; i <= interfaces::globals->max_clients; i++) {
		auto e = (player_t*)interfaces::entity_list->get_client_entity(i);

		if (!e) continue;
		if (e->health() < 0) continue;
		if (e->dormant()) continue;
		if (e->team() == local->team()) continue;

		if (e->health() > 0) {

			backtrack[i][user_cmd->command_number % 14] = backtrack_tick 
			{ 
				e->simulation_time(),
				e->bone_pos(8)
			};

			vec3_t view_direction = angle_vector(user_cmd->viewangles);
			float fov = distance_point_to_line(e->bone_pos(8), local->bone_pos(8), view_direction);

			if (best_fov > fov) {
				best_fov = fov;
				best_target = i;
			}
		}
	}
	float best_target_simulation_time = 0.f;

	if (best_target != -1) {
		float temp = FLT_MAX;
		vec3_t view_direction = angle_vector(user_cmd->viewangles);

		for (int t = 0; t < 14; ++t) {
			float fov = distance_point_to_line(backtrack[best_target][t].hitbox_position, local->bone_pos(8), view_direction);
			if (temp > fov && backtrack[best_target][t].simulation_time > local->simulation_time() - 1) {
				temp = fov;
				best_target_simulation_time = backtrack[best_target][t].simulation_time;
			}
		}

		if (user_cmd->buttons & in_attack) {
			user_cmd->tick_count = (int)(0.5f + (float)(best_target_simulation_time) / interfaces::globals->interval_per_tick);
		}
	}
}

backtrack_tick backtrack[64][14];