#include "../menu.h"
int loop_index;
void c_menu::combobox(int* feature, std::string name, std::vector< std::string > items, int amnt) {

	if (!(push_y < (pos.y + 22) || push_y >((pos.y + 4) + (height - 27))))
	{

		static bool pressed_back = false;
		static bool pressed_add = false;

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 168, push_y - 1, 10, 10)) {
			if (pressed_back&& loop_index > 0) loop_index--;
			pressed_back = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 168, push_y - 1, 10, 10) && !pressed_back)
			pressed_back = true;

		RECT size = render::get().get_text_size(render::get().menu_font, items[loop_index].c_str());

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 186 + size.right, push_y - 1, 10, 10)) {
			if (pressed_add) loop_index++;
			pressed_add = false;
		}

		if (loop_index < 0)
			loop_index = 0;

		if (loop_index > amnt)
			loop_index = amnt;

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(pos.x + 186 + size.right, push_y - 1, 10, 10) && !pressed_add)
			pressed_add = true;


		/*render::get().draw_line(pos.x + 187 + size.right, push_y + 2, pos.x + 192 + size.right, push_y + 4, color(220, 220, 220));
		render::get().draw_line(pos.x + 192 + size.right, push_y + 4, pos.x + 186 + size.right, push_y + 7, color(220, 220, 220));

		render::get().draw_line(pos.x + 174, push_y + 1, pos.x + 169, push_y + 3, color(220, 220, 220));
		render::get().draw_line(pos.x + 169, push_y + 3, pos.x + 175, push_y + 6, color(220, 220, 220));*/

		render::get().draw_arrow(pos.x + 174, push_y + 1, 3, "both", 1, size.right + 14);

		render::get().draw_text(pos.x + 182, push_y - 3, render::get().menu_font, items[loop_index].c_str(), color(220, 220, 220));
		render::get().draw_text(pos.x + 17, push_y - 2, render::get().menu_font, name.c_str(), color(220, 220, 220));

		*feature = loop_index;
	}
	push_y += 18;
}