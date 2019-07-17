#include "../../../dependencies/common_includes.hpp"
//#include "../../../dependencies/interfaces/icliententitylist.hpp"
class chams : public singleton<chams>
{
public:
	i_material * create_material(bool ignorez, bool lit, bool wireframe);
	i_material * create_material_reflective(bool ignorez, bool lit, bool wireframe);
	void InitKeyValues(key_values* kv_, std::string name_);
	void LoadFromBuffer(key_values* vk_, std::string name_, std::string buffer_);
};