#pragma once //backup
#include "../../dependencies/common_includes.hpp"
#include "../../dependencies/interfaces/textures.hpp"
#include <algorithm>
#include "../menu/menu.h"
#include "../features/visuals/visuals.hpp"
#include "../features/visuals/chams.hpp"
#include "../features/aimbot/lagcomp.hpp"
#include "../features/misc/bunnyhop.hpp"
#include "../features/aimbot/legitbot.h"
#include <Psapi.h>
#define square( x ) ( x * x )
#include <cmath>

inline float FastSqrt(float x) {
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
bool cfg::visuals_enabled = false;
bool cfg::player_name = false;
bool cfg::player_health = false;
bool cfg::player_weapon = false;
bool cfg::aimbot = false;
int cfg::aimbot_fov = 0;
int cfg::aimbot_smooth = 0;
bool cfg::cham_xqz = false;
bool cfg::rcs = false;
bool cfg::chams = false;
int cfg::chams_combo = 0;
bool cfg::bunnyhop = false;
bool cfg::lagcomp = false;
bool cfg::player_box = false;
bool cfg::clantag = false;
std::unique_ptr<vmt_hook> hooks::clientmode_hook;
std::unique_ptr<vmt_hook> hooks::panel_hook;
std::unique_ptr<vmt_hook> hooks::renderview_hook;
std::unique_ptr<vmt_hook> hooks::surface_hook;

WNDPROC hooks::wndproc_original = NULL;
void hooks::initialize( ) {
	clientmode_hook = std::make_unique<vmt_hook>( );
	panel_hook = std::make_unique<vmt_hook>( );
	renderview_hook = std::make_unique<vmt_hook>( );
	surface_hook = std::make_unique<vmt_hook>();

	clientmode_hook->setup( interfaces::clientmode );
	clientmode_hook->hook_index( 24, reinterpret_cast< void* >( create_move ) );

	panel_hook->setup( interfaces::panel );
	panel_hook->hook_index( 41, reinterpret_cast< void* >( paint_traverse ) );

	renderview_hook->setup( interfaces::render_view );
	renderview_hook->hook_index( 9, reinterpret_cast< void* >( scene_end ) );

	surface_hook->setup(interfaces::surface);
	surface_hook->hook_index(67, reinterpret_cast< void* >( lock_cursor ) );

	wndproc_original = ( WNDPROC ) SetWindowLongPtrA( FindWindow( "Valve001", NULL ), GWL_WNDPROC, ( LONG ) wndproc );

	interfaces::console->get_convar( "mat_queue_mode" )->set_value( 0 );
	interfaces::console->get_convar( "viewmodel_fov" )->callbacks.SetSize( 0 );
	interfaces::console->get_convar( "viewmodel_offset_x")->callbacks.SetSize( 0 );
	interfaces::console->get_convar( "viewmodel_offset_z")->callbacks.SetSize( 0 );
	interfaces::console->get_convar( "mat_postprocess_enable" )->set_value( 0 );
	interfaces::console->get_convar( "crosshair" )->set_value( 1 );
	render::get( ).setup_fonts( );
}

void hooks::shutdown( ) {
	panel_hook->release( );
	renderview_hook->release( );
	surface_hook->release();
	clientmode_hook->release();

	SetWindowLongPtrA( FindWindow( "Valve001", NULL ), GWL_WNDPROC, ( LONG ) wndproc_original );
}

DWORD WaitOnModuleHandle(std::string moduleName) {
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle) {
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}
bool bCompare(const BYTE * Data, const BYTE * Mask, const char* szMask) {
	for (; *szMask; ++szMask, ++Mask, ++Data) {
		if (*szMask == 'x' && *Mask != *Data)
			return false;
	}
	return (*szMask) == 0;
}
DWORD pattern_scan(std::string moduleName, BYTE * Mask, const char* szMask) {
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1) {
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
			return (DWORD)(Address + c);
	}
	return 0;
}
void clan_tag(const char* tag) {
	static auto set_clan_tag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)(pattern_scan("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx"))));
	if (cfg::clantag)
		set_clan_tag(tag, tag);
	else
		set_clan_tag("", "");
}
void animated_tag() {
	static std::string cur_clantag = "polialegit";
	static float old_curtime;

	if (interfaces::globals->cur_time - old_curtime >= 0.5f) {
		cur_clantag += cur_clantag.at(0);
		cur_clantag.erase(0, 1);

		clan_tag(cur_clantag.c_str());
		old_curtime = interfaces::globals->cur_time;
	}
}
vec3_t m_oldangle;
bool __stdcall hooks::create_move(float frame_time, c_usercmd* user_cmd) {
	static auto original_fn = reinterpret_cast<create_move_fn>(clientmode_hook->get_original(24))(interfaces::clientmode, frame_time, user_cmd);
	if (!user_cmd || !user_cmd->command_number)
		return original_fn;

	if (!interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()))
		return original_fn;

	player_t * local = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());
	if (!local) return false;
	if (interfaces::engine->is_connected() && interfaces::engine->is_in_game()) {

		if (cfg::bunnyhop) {
			bhop::get().bunnyhop(user_cmd);
			bhop::get().autostrafe(user_cmd);
		}

		animated_tag();

		aimbot::get().initalize(user_cmd);

		if (cfg::aimbot) {
			if (cfg::lagcomp)
				lagcomp::get().handle(local, user_cmd);

			if (cfg::rcs) {
				user_cmd->viewangles -= local->punchangle() * 2.f;
			}
		}
		utilities::math::fix_movement(user_cmd);
		utilities::math::normalize_view(user_cmd->viewangles);
	}
	return false;
}

void __stdcall hooks::paint_traverse( unsigned int panel, bool force_repaint, bool allow_force ) {
	std::string panel_name = interfaces::panel->get_panel_name( panel );

	reinterpret_cast< paint_traverse_fn >( panel_hook->get_original( 41 ) )( interfaces::panel, panel, force_repaint, allow_force );

	static unsigned int _panel = 0;
	static bool once = false;

	if ( !once ) {
		PCHAR panel_char = ( PCHAR ) interfaces::panel->get_panel_name( panel );
		if ( strstr( panel_char, "MatSystemTopPanel" ) ) {
			_panel = panel;
			once = true;
		}
	}
	else if ( _panel == panel ) {
		c_visuals::get().run();
		c_menu::get().draw();



	}
}
void __stdcall hooks::scene_end( ) {
	for (int i = 0; i <= 64; i++) {
		player_t* e = (player_t*)interfaces::entity_list->get_client_entity(i);
		if (!e) continue;
		if (e->dormant()) continue;
		if (!e->health()) continue;

		static i_material* texture = chams::get().create_material(false, true, false);
		static i_material* texture_xqz = chams::get().create_material(true, true, false);

		float color[3] = { 0.f };
		color[0] = 255 / 255.f;
		color[1] = 255 / 255.f;
		color[2] = 255 / 255.f;
		if (cfg::visuals_enabled) {
			switch (cfg::chams_combo) {
			case 0: break;
			case 1: interfaces::model_render->override_material(texture); break;
			case 2: interfaces::model_render->override_material(texture_xqz); break;
			}
			interfaces::render_view->color_modulation(color);
			e->draw_model(0x1, 255);
			interfaces::model_render->override_material(nullptr);
		}
	}

	reinterpret_cast< scene_end_fn >(renderview_hook->get_original(9))(interfaces::render_view);
}
LRESULT __stdcall hooks::wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) {
	return CallWindowProcA( wndproc_original, hwnd, message, wparam, lparam );
}

void __stdcall hooks::lock_cursor() {
	reinterpret_cast<lock_cursor_fn>(surface_hook->get_original(67))(interfaces::surface);

	if (c_menu::get().menu_opened) {
		interfaces::surface->unlock_cursor();
		return;
	}
}
