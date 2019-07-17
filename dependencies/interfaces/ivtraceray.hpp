/*#pragma once
#include "../common_includes.hpp"

struct Ray_t
{
	__declspec(align(16)) vec3_t  m_Start;
	__declspec(align(16)) vec3_t  m_Delta;
	__declspec(align(16)) vec3_t  m_StartOffset;
	__declspec(align(16)) vec3_t  m_Extents;
	//without your matrix3x4
	bool	m_IsRay;
	bool	m_IsSwept;

	void Init(vec3_t& vecStart, vec3_t& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.length_sqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;

		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}
	void Init(vec3_t const& start, vec3_t const& end, vec3_t const& mins, vec3_t const& maxs) {
		m_Delta.x = end.x - start.x;
		m_Delta.y = end.y - start.y;
		m_Delta.z = end.z - start.z;
		m_IsSwept = m_Delta.length_sqr() != 0;

		m_Extents.x = maxs.x - mins.x;
		m_Extents.y = maxs.y - mins.y;
		m_Extents.z = maxs.z - mins.z;

		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.length_sqr() < 1e-6);

		m_StartOffset.x = maxs.x + mins.x;
		m_StartOffset.y = maxs.y + mins.y;
		m_StartOffset.z = maxs.z + mins.z;

		m_StartOffset *= 0.5f;

		m_Start.x = start.x + m_StartOffset.x;
		m_Start.y = start.y + m_StartOffset.y;
		m_Start.z = start.z + m_StartOffset.z;

		m_StartOffset *= -1.0f;
	}
};

struct cplane_t
{
	vec3_t  normal;
	float   dist;
	BYTE    type;
	BYTE    signbits;
	BYTE    pad[2];
};

class CBaseTrace
{
public:
	vec3_t                  startpos;
	vec3_t                  endpos;
	cplane_t                plane;
	float                   fraction;
	int                             contents;
	unsigned short  dispFlags;
	bool                    allsolid;
	bool                    startsolid;
};

struct csurface_t
{
	const char*             name;
	short                   surfaceProps;
	unsigned short  flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const;
	int                             GetEntityIndex() const;
	bool                    DidHit() const;
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                             hitgroup;
	short                   physicsbone;
	unsigned short  worldSurfaceIndex;
	player_t*               m_pEnt;
	int                             hitbox;
	char shit[0x24];
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1.0f || allsolid || startsolid;
}

typedef CGameTrace trace_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};




class ITraceFilter
{
public:
	virtual bool            ShouldHitEntity(player_t* pEntity, int contentsMask) = 0;
	virtual TraceType_t     GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(player_t* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};


class CTraceWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(player_t* pEntityHandle, int contentsMask)
	{
		return false;
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

//this might be the cause idk and idc anymore rly
class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2)
	{
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	bool ShouldHitEntity(player_t* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void *passentity1;
	void *passentity2;
};

#define	MASK_ALL					(0xFFFFFFFF)
class i_engine_trace
{
public:
	int	GetPointContents(const vec3_t &vecAbsPosition, int contentsMask = MASK_ALL, player_t** ppEntity = NULL)//
	{
		typedef int(__thiscall* fnGetPointContents)(void*, const vec3_t&, int, player_t**);
		return call_vfunc<fnGetPointContents>(this, 0)(this, vecAbsPosition, contentsMask, ppEntity);
	}
	void JewishFuckingTraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* oTraceRay)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		call_vfunc<oTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* fnTraceRay)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		call_vfunc<fnTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
	void EdgeTraceRay(Ray_t &ray, CTraceFilter &filt, CGameTrace &trace, bool wall = false)
	{
		typedef void(__thiscall *OrigFn)(void *, Ray_t &, unsigned int, CTraceFilter &, CGameTrace &);
		call_vfunc<OrigFn>(this, 5)(this, ray, wall ? 0x200400B : 0x46004003, filt, trace);
	}
};
*/