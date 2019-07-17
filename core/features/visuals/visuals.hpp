#pragma once
#include "../../../dependencies/common_includes.hpp"
class c_visuals : public singleton< c_visuals > {
private:

public:
	void run();

	struct box { int x, y, w, h; };

	bool get_box(player_t* m_entity, box& result);
};