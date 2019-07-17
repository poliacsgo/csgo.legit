#include "menu.h"
#include <random>
#include <conio.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>
std::vector<vec2_t> dots[2];
typedef std::chrono::high_resolution_clock Clock;
vec2_t c_menu::mouse_pos_()
{
	POINT mouse_position;
	GetCursorPos(&mouse_position);
	ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mouse_position);
	return { static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y) };
}
bool c_menu::mouse_in_params(int x, int y, int w, int h) {
	if (mouse_pos_().x > x&& mouse_pos_().y > y&& mouse_pos_().x < w + x && mouse_pos_().y < h + y)
		return true;
	return false;
}
void c_menu::render_scrollbar(int x, int y, int width, int height) {
	int m_y = c_menu::get().mouse_pos_().y;
	int m_x = c_menu::get().mouse_pos_().x;

	static int old_pos = y;

	render::get().draw_filled_rect(x, y, width * .66666666666, height, color(70, 70, 70));

	if (GetAsyncKeyState(VK_LBUTTON) && c_menu::get().mouse_in_params(x, y, width* .66666666666, height - width)) {

		y = c_menu::get().mouse_pos_().y;
		old_pos = c_menu::get().mouse_pos_().y;
	}
	else if (!c_menu::get().mouse_in_params(x, y, width * .66666666666, width)) {
		y = old_pos;
	}

	render::get().draw_filled_rect(x, y, width * .66666666666, width, color(90, 90, 90));

	render::get().draw_filled_rect(x + (width * 0.15), y + (width * 0.35), width * 0.47, 1, color(50, 50, 50));
	render::get().draw_filled_rect(x + (width * 0.15), y + (width * 0.65), width * 0.47, 1, color(50, 50, 50));
}
void c_menu::draw()
{
	static bool _pressed = false;
	if (!_pressed && GetAsyncKeyState(VK_INSERT))
		_pressed = true;
	else if (_pressed && !GetAsyncKeyState(VK_INSERT))
	{
		_pressed = false;
		menu_opened = !menu_opened;

		interfaces::inputsystem->enable_input(!menu_opened);
	}

	if (menu_opened) {

		render::get().draw_filled_rect(pos.x, pos.y, width, height, color(35, 35, 35)); //outer box + header
		render::get().draw_outline(pos.x, pos.y, width, height, color(100, 100, 100)); //outer box outline

		render::get().draw_filled_rect(pos.x + 7, pos.y + 18, width - 14, height - 25, color(50, 50, 50)); //inner box
		render::get().draw_outline(pos.x + 7, pos.y + 18, width - 14, height - 25, color(100, 100, 100)); //inner box outline

		render::get().draw_text(pos.x + 8, pos.y + 2, render::get().menu_font, "CS:GO Legit Cheat                  by polia#9328", color(220, 220, 220));


		int y = pos.y + 19;
		int m_y = c_menu::get().mouse_pos_().y;
		int m_x = c_menu::get().mouse_pos_().x;

		static int old_pos = y;
		static bool changing_val = false;
		render::get().draw_filled_rect(pos.x + width - 19, y, 17 * .66666666666, height - 27, color(70, 70, 70));

		if (GetAsyncKeyState(VK_LBUTTON) && c_menu::get().mouse_in_params(pos.x + width - 19, y, 17 * .66666666666, height - 27 - 17)) {

			y = c_menu::get().mouse_pos_().y;
			old_pos = c_menu::get().mouse_pos_().y;
			changing_val = true;
		}
		else if (!c_menu::get().mouse_in_params(pos.x + width - 19, y, 17 * .66666666666, 17)) {
			y = old_pos;
			changing_val = false;
		}


		render::get().draw_filled_rect(pos.x + width - 19, y, 17 * .66666666666, 17, color(90, 90, 90));
		    
		render::get().draw_filled_rect(pos.x + width - 19 + (17 * 0.15), y + (17 * 0.35), 17 * 0.47, 1, color(50, 50, 50));
		render::get().draw_filled_rect(pos.x + width - 19 + (17 * 0.15), y + (17 * 0.65), 17 * 0.47, 1, color(50, 50, 50));
		 
		this->push_y = (pos.y + 22) - (y - (pos.y + 19));


		split("Visuals");

		checkbox(&cfg::visuals_enabled, "Visuals Active");
		checkbox(&cfg::player_name, "Name ESP");
		checkbox(&cfg::player_health, "Health ESP");
		checkbox(&cfg::player_box, "Box ESP");
		combobox(&cfg::chams_combo, "Chams", { "Off", "Mat", "XQZ" }, 2);

		split("Misc");  

		checkbox(&cfg::bunnyhop, "Bunnyhop");
		checkbox(&cfg::clantag, "Clantag");

		split("Aimbot");
		
		checkbox(&cfg::aimbot, "Aimbot Active");
		slider(&cfg::aimbot_fov, "FOV", 0, 3);
		checkbox(&cfg::lagcomp, "Backtracking");
		checkbox(&cfg::rcs, "Recoil Control");

	}
}