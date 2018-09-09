#pragma once
#include <fstream>
#include <sstream>
#include <functional>
// This file is auto generated. Any modification will be deleted on next LXGenerator run.

enum LXType
{
	LXType_AABB,
	LXType_BoundingBoxComponent,
	LXType_Camera,
	LXType_Component,
	LXType_CullingOptions,
	LXType_CurlRequest,
	LXType_DebugRenderer,
	LXType_DisplayMode,
	LXType_DisplayOptions,
	LXType_EEditorTool,
	LXType_Editor,
	LXType_EditorOptions,
	LXType_EditorTool,
	LXType_EngineSettings,
	LXType_EngineStats,
	LXType_Entity,
	LXType_Framebuffer,
	LXType_GL,
	LXType_GLPixelFormat,
	LXType_GLPixelType,
	LXType_GLuint,
	LXType_GUI,
	LXType_Heightfield,
	LXType_InputHandler,
	LXType_MainWindow,
	LXType_Material,
	LXType_Mesh,
	LXType_Model,
	LXType_Node,
	LXType_OSMDataProcessor,
	LXType_OSMDataProcessorSettings,
	LXType_OSMElementComponent,
	LXType_OSMElementType,
	LXType_OSMTool,
	LXType_PickingTool,
	LXType_PostEffects,
	LXType_ProgramPipeline,
	LXType_Renderer,
	LXType_RenderingStats,
	LXType_Sector,
	LXType_SectorGraphicalData,
	LXType_SectorTool,
	LXType_ShaderFamily,
	LXType_SunLight,
	LXType_TerrainEditionMode,
	LXType_TerrainTool,
	LXType_Texture,
	LXType_RenderDataManager,
	LXType_Way,
	LXType_World,
	LXType_bool,
	LXType_float,
	LXType_glmivec2,
	LXType_glmmat4,
	LXType_glmvec2,
	LXType_glmvec3,
	LXType_glmvec4,
	LXType_int,
	LXType_long,
	LXType_stdstring,
	LXType_stdvector,
	LXType_char,
	LXType_None,
};

enum PropertyFlags
{
	PropertyFlags_Hidden = 1,
	PropertyFlags_ReadOnly = 2,
	PropertyFlags_SetCallback = 4,
	PropertyFlags_NonEditable = 8,
	PropertyFlags_Adder = 16,
	PropertyFlags_Transient = 32,
	PropertyFlags_MaxValue = 64,
	PropertyFlags_MinValue = 128,
	PropertyFlags_Enum = 256,
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

#define WriteSetFunction(className, varName, varType) \
[](char* e, char* v) \
{ \
	if (g_LogsetFunctionCallbacks) \
	{ \
		std::stringstream ss; \
		ss << "Called set callback for " << #className << "::" << #varName <<"." << std::endl; \
		PRINTSTRING(ss.str()); \
	} \
	((className*)e)->Set##varName(*((varType*)v)); \
} \


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
	LXType m_Type;

	// 
	bool IsAPointer;

	// inheritance?
	LXType m_AssociatedType;

	// 
	int m_PropertyFlags;
	float m_MinValue;
	float m_MaxValue;

	std::function<void(char*, char*)> m_WriteCallback;
};
		
