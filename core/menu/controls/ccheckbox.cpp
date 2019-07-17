#include "../menu.h"

void c_menu::checkbox(bool* feature, std::string name) {

	if (!(push_y < (pos.y + 22) || push_y >((pos.y + 4) + (height - 27))))
	{
		static bool pressed = false;
		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 180, push_y - 1, 22, 12)) {
			if (pressed)
				* feature = !*feature;
			pressed = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 180, push_y - 1, 22, 12) && !pressed)
			pressed = true;

		render::get().draw_filled_rect(pos.x + 180, push_y - 1, 22, 12, color(65, 65, 65));

		if (*feature)
			render::get().draw_filled_rect(pos.x + 190, push_y - 1, 12, 12, color(50, 230, 50));
		else
			render::get().draw_filled_rect(pos.x + 180, push_y - 1, 12, 12, color(230, 50, 50));


		render::get().draw_outline(pos.x + 180, push_y - 1, 22, 12, color(0, 0, 0));
		render::get().draw_text(pos.x + 17, push_y - 2, render::get().control_font, name.c_str(), color(220, 220, 220));
	}
	push_y += 18;
}