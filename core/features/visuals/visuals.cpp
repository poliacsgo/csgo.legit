#include "../../../dependencies/common_includes.hpp"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include "../aimbot/lagcomp.hpp"
#include "../../menu/menu.h"
std::string clean_name(std::string name) {
	std::string wep = name;
	if (wep[0] == 'C') wep.erase(wep.begin());

	auto end_of_weapon = wep.find("Weapon");
	if (end_of_weapon != std::string::npos)
		wep.erase(wep.begin() + end_of_weapon, wep.begin() + end_of_weapon + 6);

	return wep;
}
void c_visuals::run() {
	box sz;
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		player_t* e = (player_t*)interfaces::entity_list->get_client_entity(i);
		player_t* local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

		if (e && !e->dormant() && e->health() && cfg::visuals_enabled) {

			if (local->team() == e->team())
				continue;

			player_info_t player_info;
			interfaces::engine->get_player_info(i, &player_info);

			if (get_box(e, sz)) {

				if (cfg::player_box) {
					render::get().draw_outline(sz.x, sz.y, sz.w, sz.h, color(180, 180, 180));
					render::get().draw_outline(sz.x - 1, sz.y - 1, sz.w + 2, sz.h + 2, color(0, 0, 0));
					render::get().draw_outline(sz.x + 1, sz.y + 1, sz.w - 2, sz.h - 2, color(0, 0, 0));
				}

				if (cfg::player_name) {
					RECT name_text_size = render::get().get_text_size(render::get().esp_font, player_info.name);
					render::get().draw_text(sz.x + (sz.w / 2) - (name_text_size.right / 2), sz.y - 13, render::get().esp_font, player_info.name, color(180, 180, 180, 255));
				}

				if (cfg::player_health) {
					auto health = e->health();

					if (health > 100)
						health = 100;

					render::get().draw_text(sz.x - 16, sz.y, render::get().esp_font, std::to_string(health), color(180, 180, 180));

				}
			}
		}
	}
}

bool c_visuals::get_box(player_t* m_entity, box& result)
{
	vec3_t  vOrigin, min, max,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	vOrigin = m_entity->abs_origin();
	auto collideable = m_entity->collideable();
	if (!collideable) return false;

	min = collideable->mins() + vOrigin;
	max = collideable->maxs() + vOrigin;

	vec3_t points[] =
	{
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	if (!render::get().world_to_screen(points[3], flb) || !render::get().world_to_screen(points[5], brt)
		|| !render::get().world_to_screen(points[0], blb) || !render::get().world_to_screen(points[4], frt)
		|| !render::get().world_to_screen(points[2], frb) || !render::get().world_to_screen(points[1], brb)
		|| !render::get().world_to_screen(points[6], blt) || !render::get().world_to_screen(points[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}