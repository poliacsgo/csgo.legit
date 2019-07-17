#pragma once
#include "../../source-sdk/structs/materials.hpp"

class i_material_system {
public:
	i_material * find_material(char const* material_name, const char* group_name, bool complain = true, const char* complain_prefix = 0) {
		using original_fn = i_material * (__thiscall*)(i_material_system*, char const*, const char*, bool, const char*);
		return (*(original_fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
	}
	i_material * create_material(const char *material, key_values* keyval) {
		using original_fn = i_material * (__thiscall*)(i_material_system*, char const*, key_values*);
		return (*(original_fn**)this)[83](this, material, keyval);
	}
	material_handle_t first_material() {
		using original_fn = material_handle_t(__thiscall*)(i_material_system*);
		return (*(original_fn**)this)[86](this);
	}
	material_handle_t next_material(material_handle_t handle) {
		using original_fn = material_handle_t(__thiscall*)(i_material_system*, material_handle_t);
		return (*(original_fn**)this)[87](this, handle);
	}
	material_handle_t invalid_material_handle() {
		using original_fn = material_handle_t(__thiscall*)(i_material_system*);
		return (*(original_fn**)this)[88](this);
	}
	i_material* get_material(material_handle_t handle) {
		using original_fn = i_material * (__thiscall*)(i_material_system*, material_handle_t);
		return (*(original_fn**)this)[89](this, handle);
	}
	int	get_materials_count() {
		using original_fn = int(__thiscall*)(i_material_system*);
		return (*(original_fn**)this)[90](this);
	}
};
class CGlowObjectManager
{
public:

	class GlowObjectDefinition_t
	{
	public:

		class player_t *m_pEntity;
		union
		{
			vec3_t m_vGlowColor;           //0x0004
			struct
			{
				float   m_flRed;           //0x0004
				float   m_flGreen;         //0x0008
				float   m_flBlue;          //0x000C
			};
		};
		float m_flGlowAlpha;
		uint8_t pad_0014[4];
		float m_flSomeFloatThingy;
		uint8_t pad_001C[4];
		float m_flAnotherFloat;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		uint8_t pad_0027[5];
		int32_t m_bPulsatingChams;
		int32_t m_nSplitScreenSlot;
		int32_t m_nNextFreeSlot;


		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t *m_GlowObjectDefinitions;
	int max_size;
	int pad;
	int size;
	GlowObjectDefinition_t *m_Unknown;
	int	currentObjects;
};