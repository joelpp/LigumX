#pragma once
#include <fstream>
#include <sstream>
#include <functional>
// This file is auto generated. Any modification will be deleted on next LXGenerator run.

enum LXType
{
#define LX_CLASS(c, p) LXType_##c,
#define LX_ENUM(c) LXType_##c,
#include "LXClassList.h"
#undef LX_CLASS
#undef LX_ENUM

	LXType_bool,
	LXType_SimpleTypesStart = LXType_bool,
	LXType_float,
	LXType_glmhighp_ivec2,
	LXType_glmivec2,
	LXType_glmmat4,
	LXType_glmvec2,
	LXType_glmvec3,
	LXType_glmvec4,
	LXType_int,
	LXType_lxInt64,
	LXType_stdstring,
	LXType_stdvector,
	LXType_char,
	LXType_Object,
	LXType_ObjectPtr,

	LXType_GLuint,

	LXType_None,
	LXType_Count,
};

enum PropertyFlags
{
	PropertyFlags_Hidden = 1 << 0,
	PropertyFlags_ReadOnly = 1 << 1,
	PropertyFlags_SetCallback = 1 << 2,
	PropertyFlags_GetCallback = 1 << 3,
	PropertyFlags_MinValue = 1 << 4,
	PropertyFlags_MaxValue = 1 << 5,
	PropertyFlags_Transient = 1 << 6,
	PropertyFlags_NonEditable = 1 << 7,
	PropertyFlags_Adder = 1 << 8,
	PropertyFlags_Enum = 1 << 9,
	PropertyFlags_NoCloning = 1 << 10,
};

template <class T> class LXFunctor
{
private:
	T* ObjectPtr;
	void (T::*MemberFunction) ();
public:
	void operator () ()
	{
		return (*this->ObjectPtr.*this->MemberFunction)();
	}
};

static bool g_LogsetFunctionCallbacks = false;

#define LX_ENABLE_WRITECALLBACK 1

#if LX_ENABLE_WRITECALLBACK
#define WriteSetFunction(className, varName, varType) \
[](char* e, char* v) \
{ \
((className*)e)->Set##varName(*((varType*)v)); \
}
#else
#define WriteSetFunction(className, varName, varType) \
[](char* e, char* v) \
{ \
}
#endif

// todo handle structs in .gen files
struct ClassPropertyData
{
	// member name
	const char* m_Name;

	// member index
	int m_Index;

	// member byte offset
	int m_Offset;

	// ??
	int debug;

	// todo : change this to ptr, primitive types, object only.
	LXType m_SimpleType;

	std::size_t m_SimpleTypeSize;

	LXType m_Type;

	// 
	bool IsAPointer;

	// inheritance?
	LXType m_AssociatedType;
	bool m_AssociatedPtr;

	// 
	int m_PropertyFlags;
	float m_MinValue;
	float m_MaxValue;

	std::function<void(char*, char*)> m_WriteCallback;
};

