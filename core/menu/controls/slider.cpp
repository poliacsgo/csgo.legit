#include "../menu.h"
int value;
void c_menu::slider(int* feature, std::string name, int min, int max) {

	if (!(push_y < (pos.y + 22) || push_y >((pos.y + 4) + (height - 27))))
	{

		static bool pressed_sub = false;
		static bool pressed_add = false;

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 175, push_y - 1, 10, 10)) {
			if (pressed_sub&& value > 0) value--;
			pressed_sub = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 175, push_y - 1, 10, 10) && !pressed_sub)
			pressed_sub = true;

		RECT size = render::get().get_text_size(render::get().menu_font, std::to_string(value).c_str());

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 191 + size.right, push_y - 1, 10, 10)) {
			if (pressed_add) value++;
			pressed_add = false;
		}

		if (value < min)
			value = min;

		if (value > max)
			value = max;

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 190 + size.right, push_y - 1, 10, 10) && !pressed_add)
			pressed_add = true;


		//render::get().draw_line(pos.x + 197 + size.right, push_y + 1, pos.x + 202 + size.right, push_y + 3, color(220, 220, 220));
		//render::get().draw_line(pos.x + 202 + size.right, push_y + 3, pos.x + 197 + size.right, push_y + 5, color(220, 220, 220));

		//render::get().draw_line(pos.x + 184, push_y + 1, pos.x + 179, push_y + 3, color(220, 220, 220));
		//render::get().draw_line(pos.x + 179, push_y + 3, pos.x + 184, push_y + 5, color(220, 220, 220));

		render::get().draw_arrow(pos.x + 178, push_y, 3, "both", 1, size.right + 14);


		render::get().draw_text(pos.x + 187, push_y - 3, render::get().menu_font, std::to_string(value).c_str(), color(220, 220, 220));
		render::get().draw_text(pos.x + 17, push_y - 2, render::get().menu_font, name.c_str(), color(220, 220, 220));

		*feature = value;
	}
	push_y += 18;
}