#pragma once
#include <vector>
#include <iostream>
#include <functional>
#include "../../dependencies/utilities/render.hpp"
#include "../../dependencies/common_includes.hpp"

class c_menu : public singleton<c_menu> {
public:
	bool menu_opened;
	vec2_t mouse_pos_();
	bool mouse_in_params(int x, int y, int x2, int y2);
	void draw();



	void checkbox(bool* feature, std::string name);
	void combobox(int* feature, std::string name, std::vector< std::string > items, int size);
	void slider(int* feature, std::string name, int min, int max);
	void split(std::string name);


	void render_scrollbar(int x, int y, int width, int height);
	int push_y = 0;
private:
	vec3_t pos = vec3_t(650, 300, 0);

	int width = 235;
	int height = 310;
};
