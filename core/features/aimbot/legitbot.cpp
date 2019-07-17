#include "legitbot.h"

void aimbot::initalize(c_usercmd* user_cmd) {
	player_t* local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());
	if (!local) return;

	if (cfg::aimbot) {
		run_aimbot(local, user_cmd);
	}
}
void aimbot::run_aimbot(player_t* local, c_usercmd* user_cmd) {
	weapon_t* weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle((uintptr_t)local->active_weapon_handle());
	if (!weapon) return;
	for (int i = 0; i <= 64; i++) {
		player_t* e = (player_t*)interfaces::entity_list->get_client_entity(i);
		if (!e || weapon->notgun()) continue;
		if (e->dormant() || !e->health()) continue;
		if (e->has_gun_game_immunity()) continue;
		if (local->team() == e->team()) continue;

		vec3_t origin = local->bone_pos(8), point = e->bone_pos(8);

		vec3_t enemy_direction = normalize(calc_angle(origin, point));
		if ((user_cmd->viewangles - enemy_direction).length_2d() <= cfg::aimbot_fov && enemy_direction != vec3_t(0, 0, 0)) {
			if (user_cmd->buttons & in_attack) {
				user_cmd->viewangles = enemy_direction;
			}
		}
	}
}