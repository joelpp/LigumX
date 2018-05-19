#include "stdafx.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions

#include "DisplayOptions.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData DisplayOptions::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(DisplayOptions, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(DisplayOptions, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "UseSkyLighting", PIDX_UseSkyLighting, offsetof(DisplayOptions, m_UseSkyLighting), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderTerrain", PIDX_RenderTerrain, offsetof(DisplayOptions, m_RenderTerrain), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DrawSky", PIDX_DrawSky, offsetof(DisplayOptions, m_DrawSky), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "WireframeRendering", PIDX_WireframeRendering, offsetof(DisplayOptions, m_WireframeRendering), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "UseLighting", PIDX_UseLighting, offsetof(DisplayOptions, m_UseLighting), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowNormals", PIDX_ShowNormals, offsetof(DisplayOptions, m_ShowNormals), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowSpecular", PIDX_ShowSpecular, offsetof(DisplayOptions, m_ShowSpecular), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowDiffuse", PIDX_ShowDiffuse, offsetof(DisplayOptions, m_ShowDiffuse), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowAmbient", PIDX_ShowAmbient, offsetof(DisplayOptions, m_ShowAmbient), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowFPS", PIDX_ShowFPS, offsetof(DisplayOptions, m_ShowFPS), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowDepth", PIDX_ShowDepth, offsetof(DisplayOptions, m_ShowDepth), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "LinearizeDepth", PIDX_LinearizeDepth, offsetof(DisplayOptions, m_LinearizeDepth), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "BlinnPhongShading", PIDX_BlinnPhongShading, offsetof(DisplayOptions, m_BlinnPhongShading), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderTextureOverlay", PIDX_RenderTextureOverlay, offsetof(DisplayOptions, m_RenderTextureOverlay), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderOpaque", PIDX_RenderOpaque, offsetof(DisplayOptions, m_RenderOpaque), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderShadows", PIDX_RenderShadows, offsetof(DisplayOptions, m_RenderShadows), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "OutputGLErrors", PIDX_OutputGLErrors, offsetof(DisplayOptions, m_OutputGLErrors), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DisplayDebugModels", PIDX_DisplayDebugModels, offsetof(DisplayOptions, m_DisplayDebugModels), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DisplayMode", PIDX_DisplayMode, offsetof(DisplayOptions, m_DisplayMode), 0, LXType_DisplayMode, false, LXType_None, PropertyFlags_Enum, 0, 0, }, 
};
bool DisplayOptions::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
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
	m_ObjectID = g_ObjectManager->GetNewObjectID();
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
