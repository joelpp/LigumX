#include "stdafx.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions

#include "DisplayOptions.h"
const ClassPropertyData DisplayOptions::g_Properties[] = 
{
{ "UseSkyLighting", PIDX_UseSkyLighting, offsetof(DisplayOptions, m_UseSkyLighting), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "RenderTerrain", PIDX_RenderTerrain, offsetof(DisplayOptions, m_RenderTerrain), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DrawSky", PIDX_DrawSky, offsetof(DisplayOptions, m_DrawSky), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "WireframeRendering", PIDX_WireframeRendering, offsetof(DisplayOptions, m_WireframeRendering), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "UseLighting", PIDX_UseLighting, offsetof(DisplayOptions, m_UseLighting), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowNormals", PIDX_ShowNormals, offsetof(DisplayOptions, m_ShowNormals), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowUVs", PIDX_ShowUVs, offsetof(DisplayOptions, m_ShowUVs), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowSpecular", PIDX_ShowSpecular, offsetof(DisplayOptions, m_ShowSpecular), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowDiffuse", PIDX_ShowDiffuse, offsetof(DisplayOptions, m_ShowDiffuse), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowAmbient", PIDX_ShowAmbient, offsetof(DisplayOptions, m_ShowAmbient), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowFPS", PIDX_ShowFPS, offsetof(DisplayOptions, m_ShowFPS), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ShowDepth", PIDX_ShowDepth, offsetof(DisplayOptions, m_ShowDepth), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "LinearizeDepth", PIDX_LinearizeDepth, offsetof(DisplayOptions, m_LinearizeDepth), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "BlinnPhongShading", PIDX_BlinnPhongShading, offsetof(DisplayOptions, m_BlinnPhongShading), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "RenderTextureOverlay", PIDX_RenderTextureOverlay, offsetof(DisplayOptions, m_RenderTextureOverlay), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "RenderOpaque", PIDX_RenderOpaque, offsetof(DisplayOptions, m_RenderOpaque), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "RenderShadows", PIDX_RenderShadows, offsetof(DisplayOptions, m_RenderShadows), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "OutputGLErrors", PIDX_OutputGLErrors, offsetof(DisplayOptions, m_OutputGLErrors), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayDebugModels", PIDX_DisplayDebugModels, offsetof(DisplayOptions, m_DisplayDebugModels), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "PickingEnabled", PIDX_PickingEnabled, offsetof(DisplayOptions, m_PickingEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "DisplayMode", PIDX_DisplayMode, offsetof(DisplayOptions, m_DisplayMode), 0, LXType_Object, sizeof(DisplayMode), LXType_DisplayMode, false, LXType_None, false, PropertyFlags_Enum, 0, 0, 0,}, 
};
bool DisplayOptions::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DisplayOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("UseSkyLighting", m_UseSkyLighting);
	LXIMGUI_SHOW_BOOL("RenderTerrain", m_RenderTerrain);
	LXIMGUI_SHOW_BOOL("DrawSky", m_DrawSky);
	LXIMGUI_SHOW_BOOL("WireframeRendering", m_WireframeRendering);
	LXIMGUI_SHOW_BOOL("UseLighting", m_UseLighting);
	LXIMGUI_SHOW_BOOL("ShowNormals", m_ShowNormals);
	LXIMGUI_SHOW_BOOL("ShowUVs", m_ShowUVs);
	LXIMGUI_SHOW_BOOL("ShowSpecular", m_ShowSpecular);
	LXIMGUI_SHOW_BOOL("ShowDiffuse", m_ShowDiffuse);
	LXIMGUI_SHOW_BOOL("ShowAmbient", m_ShowAmbient);
	LXIMGUI_SHOW_BOOL("ShowFPS", m_ShowFPS);
	LXIMGUI_SHOW_BOOL("ShowDepth", m_ShowDepth);
	LXIMGUI_SHOW_BOOL("LinearizeDepth", m_LinearizeDepth);
	LXIMGUI_SHOW_BOOL("BlinnPhongShading", m_BlinnPhongShading);
	LXIMGUI_SHOW_BOOL("RenderTextureOverlay", m_RenderTextureOverlay);
	LXIMGUI_SHOW_BOOL("RenderOpaque", m_RenderOpaque);
	LXIMGUI_SHOW_BOOL("RenderShadows", m_RenderShadows);
	LXIMGUI_SHOW_BOOL("OutputGLErrors", m_OutputGLErrors);
	LXIMGUI_SHOW_BOOL("DisplayDebugModels", m_DisplayDebugModels);
	LXIMGUI_SHOW_BOOL("PickingEnabled", m_PickingEnabled);
	return true;
}
const char* DisplayOptions::GetTypeName()
{
	return ClassName;
}
const std::string EnumValues_DisplayMode[] = 
{
"Final",
"Depth",
"UV",
"Diffuse",
"Normals",
};

const DisplayMode Indirection_DisplayMode[] =
{
	DisplayMode_Final,
	DisplayMode_Depth,
	DisplayMode_UV,
	DisplayMode_Diffuse,
	DisplayMode_Normals,
};

#pragma endregion  CLASS_SOURCE DisplayOptions

DisplayOptions::DisplayOptions()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}

//void DisplayOptions::Serialize(bool writing)
//{
//	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
//	std::string fileName = "DisplayOptions.LXobj";
//
//	int fileMask = writing ? std::ios::out : std::ios::in;
//	std::fstream objectStream(basePath + fileName, fileMask);
//
//	if (objectStream.is_open())
//	{
//		if (writing)
//		{
//			objectStream << m_UseSkyLighting << std::endl;
//			objectStream << m_RenderTerrain << std::endl;
//			objectStream << m_DrawSky << std::endl;
//		}
//		else
//		{
//			objectStream >> m_UseSkyLighting;
//			objectStream >> m_RenderTerrain;
//			objectStream >> m_DrawSky;
//		}
//	}
//
//
//}
