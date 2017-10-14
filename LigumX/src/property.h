#pragma once
#include <fstream>

// if you change something here don't forget to change it in the LXGenerator also

// todo would be nice to gen this from a .gen file
// bonus points : that is common to LXGenerator and LigumX

enum LXType
{
	LXType_int,
	LXType_float,
	LXType_bool,
	
	LXType_stdvector,
	LXType_stdstring,

	LXType_glmvec2,
	LXType_glmvec3,
	LXType_glmvec4,
	LXType_glmivec2,
	LXType_glmmat4,

	LXType_Texture,

	LXType_GLuint,
	LXType_GLPixelFormat,

	// these are mostly a test. maybe we shouldnt get so specific and instead use LXType_Other
	// still doing it just to see what happens
	LXType_Entity,
	LXType_Material,
	LXType_Component,
	LXType_ProgramPipeline,
	LXType_PostEffects,
	LXType_DisplayOptions,
	LXType_ProjectionType,
	LXType_Model,
	LXType_AABB,

	LXType_None,
	LXType_NumTypes
};

// keep in sync with generator
// todo : replace with bitset if it gets too large (same with osmelement!)
enum PropertyFlags
{
	PropertyFlags_Hidden = 1,
	PropertyFlags_ReadOnly = 2,
	PropertyFlags_SetCallback = 4,
	PropertyFlags_Transient = 8,
	PropertyFlags_NumPropertyFlags
};

// todo handle structs in .gen files
struct ClassPropertyData
{
	const char* m_Name;
	int m_Offset;
	int debug;
	LXType m_Type;
	bool IsAPointer;
	LXType m_AssociatedType;
	int m_PropertyFlags;
	float m_MinValue;
	float m_MaxValue;
};
