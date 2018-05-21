#pragma once
#include <fstream>
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


// todo handle structs in .gen files
struct ClassPropertyData
{
	const char* m_Name;
	int m_Index;
	int m_Offset;
	int debug;
	LXType m_Type;
	bool IsAPointer;
	LXType m_AssociatedType;
	int m_PropertyFlags;
	float m_MinValue;
	float m_MaxValue;
};
		
