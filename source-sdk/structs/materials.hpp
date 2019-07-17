#pragma once

struct PreviewImageRetVal_t;
struct MaterialPropertyTypes_t;
class IMaterialVar;
class ImageFormat;

typedef int64_t VertexFormat_t;

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
};


typedef unsigned short MaterialHandle_t;
using material_handle_t = unsigned short;
class i_material {
public:
	virtual const char *    GetName() const = 0;
	virtual const char *    GetTextureGroupName() const = 0;

	virtual PreviewImageRetVal_t GetPreviewImageProperties(int *width, int *height, ImageFormat *imageFormat, bool* isTranslucent) const = 0;

	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char *data, int width, int height, ImageFormat imageFormat) const = 0;

	virtual int             GetMappingWidth() = 0;
	virtual int             GetMappingHeight() = 0;

	virtual int             GetNumAnimationFrames() = 0;

	virtual bool            InMaterialPage(void) = 0;
	virtual void            GetMaterialOffset(float *pOffset) = 0;
	virtual void            GetMaterialScale(float *pScale) = 0;
	virtual i_material       *GetMaterialPage(void) = 0;

	virtual IMaterialVar *  FindVar(const char *varName, bool *found, bool complain = true) = 0;

	virtual void            IncrementReferenceCount(void) = 0;
	virtual void            DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	virtual int             GetEnumerationID(void) const = 0;

	virtual void            GetLowResColorSample(float s, float t, float *color) const = 0;

	virtual void            RecomputeStateSnapshots() = 0;

	virtual bool            IsTranslucent() = 0;

	virtual bool            IsAlphaTested() = 0;

	virtual bool            IsVertexLit() = 0;

	virtual VertexFormat_t  GetVertexFormat() const = 0;

	virtual bool            HasProxy(void) const = 0;

	virtual bool            UsesEnvCubemap(void) = 0;

	virtual bool            NeedsTangentSpace(void) = 0;

	virtual bool            NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool            NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	virtual bool            NeedsSoftwareSkinning(void) = 0;

	virtual void            set_alpha(float alpha) = 0;
	virtual void            set_color(float r, float g, float b) = 0;

	virtual void            SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool            GetMaterialVarFlag(MaterialVarFlags_t flag) = 0;

	virtual void            GetReflectivity(vec3_t& reflect) = 0;

	virtual bool            GetPropertyFlag(MaterialPropertyTypes_t type) = 0;

	virtual bool            IsTwoSided() = 0;

	virtual void            SetShader(const char *pShaderName) = 0;

	virtual int             GetNumPasses(void) = 0;

	virtual int             GetTextureMemoryBytes(void) = 0;

	virtual void            Refresh() = 0;

	virtual bool            NeedsLightmapBlendAlpha(void) = 0;

	virtual bool            NeedsSoftwareLighting(void) = 0;

	virtual int             ShaderParamCount() const = 0;
	virtual IMaterialVar    **GetShaderParams(void) = 0;

	virtual bool            IsErrorMaterial() const = 0;

	virtual void            Unused() = 0;

	virtual float           GetAlphaModulation() = 0;
	virtual void            GetColorModulation(float *r, float *g, float *b) = 0;

	virtual bool            IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;

	virtual IMaterialVar *  FindVarFast(char const *pVarName, unsigned int *pToken) = 0;

	virtual void            SetShaderAndParams(void *pKeyValues) = 0;
	virtual const char *    GetShaderName() const = 0;

	virtual void            DeleteIfUnreferenced() = 0;

	virtual bool            IsSpriteCard() = 0;

	virtual void            CallBindProxy(void *proxyData) = 0;

	virtual void            RefreshPreservingMaterialVars() = 0;

	virtual bool            WasReloadedFromWhitelist() = 0;

	virtual bool            SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int             GetReferenceCount() const = 0;
};