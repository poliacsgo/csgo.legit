#include "../menu.h"

void c_menu::split(std::string name) {
	if (!(push_y < (pos.y + 18) || push_y > ((pos.y + 4) + (height - 27))))
	{
		render::get().draw_filled_rect(pos.x + 28, push_y + 10, 160, 1, color(90, 90, 90));

		RECT text_size = render().get().get_text_size(render::get().menu_font, name.c_str());
		render::get().draw_filled_rect((((pos.x + 28) + (160 / 2)) - (text_size.right / 2)) - 3, push_y + 9, text_size.right + 6, 3, color(50, 50, 50));

		render::get().draw_text((((pos.x + 28) + (160 / 2)) - (text_size.right / 2)), (push_y + 2), render::get().menu_font, name.c_str(), color(220, 220, 220));
	}
	push_y += 25;
}