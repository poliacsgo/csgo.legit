#pragma once

class i_render_view
{
public:
	void set_blend(float alpha) {
		using original_fn = void(__thiscall*)(i_render_view*, float);
		return (*(original_fn**)this)[4](this, alpha);
	}

	void color_modulation(float const* colors)
	{
		using original_fn = void(__thiscall*)(i_render_view*, float const*);
		return (*(original_fn**)this)[6](this, colors);
	}
};