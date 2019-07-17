#include "../../../dependencies/common_includes.hpp"
class bhop : public singleton<bhop> {
public:
	void bunnyhop(c_usercmd* user_cmd);
	void autostrafe(c_usercmd* user_cmd);
};