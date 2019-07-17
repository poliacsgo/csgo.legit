#pragma once
#include <string>
#include "singleton.hpp"
#include "../interfaces/interfaces.hpp"

enum font_flags {
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

class render : public singleton<render> {
public: // fonts
	DWORD control_font;
	DWORD menu_font;
	DWORD esp_font;

public: // basic renderer
	void setup_fonts( ) {
		static bool _o = false;
		if ( !_o ) {
			menu_font = interfaces::surface->font_create();
			control_font = interfaces::surface->font_create();
			esp_font = interfaces::surface->font_create();

			interfaces::surface->set_font_glyph(menu_font, "Arial", 14, 550, 0, 0, font_flags::fontflag_antialias);
			interfaces::surface->set_font_glyph(control_font, "Arial", 14, 550, 0, 0, font_flags::fontflag_antialias);
			interfaces::surface->set_font_glyph(esp_font, "Roboto", 11, 500, 0, 0, font_flags::fontflag_dropshadow | font_flags::fontflag_antialias);
			_o = true;
		}
	}
	void draw_line( int x1, int y1, int x2, int y2, color colour ) {
		interfaces::surface->set_drawing_color( colour.r, colour.g, colour.b, colour.a );
		interfaces::surface->draw_line( x1, y1, x2, y2 );
	}
	void draw_text(int x, int y, unsigned long font, std::string string, color colour) {
		const auto converted_text = std::wstring(string.begin(), string.end()); int width, height;
		interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);
		interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_text_font(font);
		interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
	}
	__forceinline bool world_to_screen(vec3_t In, vec3_t& Out) {
		matrix3x4_t ViewMatrix = interfaces::engine->GetMatrix();
		Out.x = ViewMatrix.Matrix[0] * In.x + ViewMatrix.Matrix[1] * In.y + ViewMatrix.Matrix[2] * In.z + ViewMatrix.Matrix[3];
		Out.y = ViewMatrix.Matrix[4] * In.x + ViewMatrix.Matrix[5] * In.y + ViewMatrix.Matrix[6] * In.z + ViewMatrix.Matrix[7];
		Out.z = ViewMatrix.Matrix[12] * In.x + ViewMatrix.Matrix[13] * In.y + ViewMatrix.Matrix[14] * In.z + ViewMatrix.Matrix[15];

		if (Out.z < 0.01f) return false;
		float Inverse = 1.f / Out.z;

		Out.x *= Inverse;
		Out.y *= Inverse;

		int Width, Height;
		interfaces::engine->get_screen_size(Width, Height);
		auto X = Width / 2;
		auto Y = Height / 2;

		X += 0.5 * Out.x * Width + 0.5;
		Y -= 0.5 * Out.y * Height + 0.5;

		Out.x = X;
		Out.y = Y;
		return true;
	}
	void draw_filled_rect( int x, int y, int w, int h, color colour ) {
		interfaces::surface->set_drawing_color( colour.r, colour.g, colour.b, colour.a );
		interfaces::surface->draw_filled_rectangle( x, y, w, h );
	}
	void draw_outline( int x, int y, int w, int h, color colour ) {
		interfaces::surface->set_drawing_color( colour.r, colour.g, colour.b, colour.a );
		interfaces::surface->draw_outlined_rect( x, y, w, h );
	}
	RECT get_text_size(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		interfaces::surface->get_text_size(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}
	vec2_t get_screen_size(vec2_t area)
	{
		static int old_w, old_h;
		interfaces::engine->get_screen_size((int&)area.x, (int&) area.y);

		if ((int&) area.x != old_w || (int&) area.y != old_h)
		{
			old_w = (int&) area.x;
			old_h = (int&) area.y;
			return vec2_t(old_w, old_h);
		}
		return area;
	}
	void draw_arrow(int x, int y, int size, std::string dir, int height, int space) {

		if (dir == "left") {
			render::get().draw_line(x, y + height, x - size, y + size, color(220, 220, 220));
			render::get().draw_line(x - size, y + size, x, (y + (size * 2)) - height, color(220, 220, 220));
		}
		else if (dir == "right") {
			render::get().draw_line(x, y + height, x + size, y + size, color(220, 220, 220));
			render::get().draw_line(x + size, y + size, x, y + (size * 2) - height,  color(220, 220, 220));
		}
		else if (dir == "both") {
			render::get().draw_line(x, y + height, x - size, y + size, color(220, 220, 220));
			render::get().draw_line(x - size, y + size, x, (y + (size * 2)) - height, color(220, 220, 220));

			x += space;

			render::get().draw_line(x, y + height, x + size, y + size, color(220, 220, 220));
			render::get().draw_line(x + size, y + size, x, y + (size * 2) - height, color(220, 220, 220));
		}
	}
};