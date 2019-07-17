#include "utilities.hpp"
#include "../common_includes.hpp"
#define square( x ) ( x * x )
#include <cmath>

#ifdef NDEBUG
#define Assert( _exp ) ((void)0)
#else
#define Assert(x)
#endif
inline float FastSqrt(float x) {
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
std::uint8_t* utilities::pattern_scan( void* module, const char* signature ) {
	static auto pattern_to_byte = [ ] ( const char* pattern ) {
		auto bytes = std::vector<int> {};
		auto start = const_cast< char* >( pattern );
		auto end = const_cast< char* >( pattern ) + strlen( pattern );

		for ( auto current = start; current < end; ++current ) {
			if ( *current == '?' ) {
				++current;
				if ( *current == '?' )
					++current;
				bytes.push_back( -1 );
			}
			else {
				bytes.push_back( strtoul( current, &current, 16 ) );
			}
		}
		return bytes;
	};

	auto dos_headers = ( PIMAGE_DOS_HEADER ) module;
	auto nt_headers = ( PIMAGE_NT_HEADERS ) ( ( std::uint8_t* )module + dos_headers->e_lfanew );

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte( signature );
	auto scan_bytes = reinterpret_cast< std::uint8_t* >( module );

	auto s = pattern_bytes.size( );
	auto d = pattern_bytes.data( );

	for ( auto i = 0ul; i < size_of_image - s; ++i ) {
		bool found = true;
		for ( auto j = 0ul; j < s; ++j ) {
			if (scan_bytes[ i + j ] != d [ j ] && d [ j ] != -1 ) {
				found = false;
				break;
			}
		}
		if ( found ) {
			return &scan_bytes[ i ];
		}
	}
	return nullptr;
}
void* utilities::game::capture_interface( const char* mod, const char* iface ) {
	using fn_capture_iface_t = void*( *)( const char*, int* );
	auto fn_addr = ( fn_capture_iface_t ) GetProcAddress( GetModuleHandleA( mod ), "CreateInterface" );

	auto iface_addr = fn_addr( iface, nullptr );
	//printf( "interface %s found at 0x%p\n", iface, fn_addr( iface, nullptr ) );

	return iface_addr;
}

void utilities::math::normalize_view(vec3_t &angle) {
	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89) angle.x = 89;
	if (angle.x < -89) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;
}
vec3_t utilities::math::normalize_angle(vec3_t angle)
{
	while (angle.x > 89.f)
	{
		angle.x -= 180.f;
	}
	while (angle.x < -89.f)
	{
		angle.x += 180.f;
	}
	if (angle.y > 180)
	{
		angle.y -= (round(angle.y / 360) * 360.f);
	}
	else if (angle.y < -180)
	{
		angle.y += (round(angle.y / 360) * -360.f);
	}
	if ((angle.z > 50) || (angle.z < 50))
	{
		angle.z = 0;
	}
	return angle;
}
void utilities::math::vector_angles(const vec3_t& forward, vec3_t &angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
void utilities::math::fix_movement(c_usercmd* cmd)
{
	auto local_player = interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	if (!local_player) return;

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.f, 450.f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.f, 450.f);
	cmd->upmove = std::clamp(cmd->upmove, -320.f, 320.f);

	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.f, 89.f);

	vec3_t viewangles;
	viewangles = interfaces::engine->get_view_angles();

	vec3_t vecMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	vec3_t angMove;
	utilities::math::vector_angles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - viewangles.y + angMove.y);

	cmd->forwardmove = cos(yaw) * speed;
	cmd->sidemove = sin(yaw) * speed;

	cmd->viewangles = utilities::math::normalize_angle(cmd->viewangles);

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->forwardmove *= -1;
}
float utilities::math::distance_point_to_line(vec3_t point, vec3_t origin, vec3_t direction) {
	auto delta = point - origin;

	auto temp = delta.dot(direction) / (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if (temp < 0.000001f)
		return FLT_MAX;

	auto temp_meme = origin + (direction * temp);
	return (point - temp_meme).length();
}

vec3_t utilities::math::angle_vector(vec3_t to_convert) {
	auto y_sin = sin(to_convert.y / 180.f * static_cast<float>(M_PI));
	auto y_cos = cos(to_convert.y / 180.f * static_cast<float>(M_PI));

	auto x_sin = sin(to_convert.x / 180.f * static_cast<float>(M_PI));
	auto x_cos = cos(to_convert.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(x_cos * y_cos, x_cos * y_sin, -x_sin);
}
void utilities::math::angle_vectors_v2(const vec3_t &angles, vec3_t *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles.y));
	cy = cos(DEG2RAD(angles.y));
	sp = sin(DEG2RAD(angles.x));
	cp = cos(DEG2RAD(angles.x));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}
vec3_t utilities::math::calc_angle(const vec3_t& in, vec3_t out)
{
	double delta[3] = { (in[0] - out[0]), (in[1] - out[1]), (in[2] - out[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	vec3_t ret = vec3_t();
	ret.x = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
	ret.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	ret.z = 0.0f;

	if (delta[0] >= 0.0)
		ret.y += 180.0f;
	return ret;
}
void utilities::math::clamp_movement(float fMaxSpeed, c_usercmd* user_cmd) {
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt(square(user_cmd->forwardmove) + square(user_cmd->sidemove) + square(user_cmd->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (user_cmd->buttons & in_duck)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	user_cmd->forwardmove *= fRatio;
	user_cmd->sidemove *= fRatio;
	user_cmd->upmove *= fRatio;
}