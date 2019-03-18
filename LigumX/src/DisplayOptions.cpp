#include "stdafx.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions

#include "DisplayOptions.h"
#include "serializer.h"
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
void DisplayOptions::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool("UseSkyLighting", m_UseSkyLighting);
	serializer.SerializeBool("RenderTerrain", m_RenderTerrain);
	serializer.SerializeBool("DrawSky", m_DrawSky);
	serializer.SerializeBool("WireframeRendering", m_WireframeRendering);
	serializer.SerializeBool("UseLighting", m_UseLighting);
	serializer.SerializeBool("ShowNormals", m_ShowNormals);
	serializer.SerializeBool("ShowUVs", m_ShowUVs);
	serializer.SerializeBool("ShowSpecular", m_ShowSpecular);
	serializer.SerializeBool("ShowDiffuse", m_ShowDiffuse);
	serializer.SerializeBool("ShowAmbient", m_ShowAmbient);
	serializer.SerializeBool("ShowFPS", m_ShowFPS);
	serializer.SerializeBool("ShowDepth", m_ShowDepth);
	serializer.SerializeBool("LinearizeDepth", m_LinearizeDepth);
	serializer.SerializeBool("BlinnPhongShading", m_BlinnPhongShading);
	serializer.SerializeBool("RenderTextureOverlay", m_RenderTextureOverlay);
	serializer.SerializeBool("RenderOpaque", m_RenderOpaque);
	serializer.SerializeBool("RenderShadows", m_RenderShadows);
	serializer.SerializeBool("OutputGLErrors", m_OutputGLErrors);
	serializer.SerializeBool("DisplayDebugModels", m_DisplayDebugModels);
	serializer.SerializeBool("PickingEnabled", m_PickingEnabled);
}
bool DisplayOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DisplayOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("UseSkyLighting", m_UseSkyLighting  );
	ImguiHelpers::ShowBool("RenderTerrain", m_RenderTerrain  );
	ImguiHelpers::ShowBool("DrawSky", m_DrawSky  );
	ImguiHelpers::ShowBool("WireframeRendering", m_WireframeRendering  );
	ImguiHelpers::ShowBool("UseLighting", m_UseLighting  );
	ImguiHelpers::ShowBool("ShowNormals", m_ShowNormals  );
	ImguiHelpers::ShowBool("ShowUVs", m_ShowUVs  );
	ImguiHelpers::ShowBool("ShowSpecular", m_ShowSpecular  );
	ImguiHelpers::ShowBool("ShowDiffuse", m_ShowDiffuse  );
	ImguiHelpers::ShowBool("ShowAmbient", m_ShowAmbient  );
	ImguiHelpers::ShowBool("ShowFPS", m_ShowFPS  );
	ImguiHelpers::ShowBool("ShowDepth", m_ShowDepth  );
	ImguiHelpers::ShowBool("LinearizeDepth", m_LinearizeDepth  );
	ImguiHelpers::ShowBool("BlinnPhongShading", m_BlinnPhongShading  );
	ImguiHelpers::ShowBool("RenderTextureOverlay", m_RenderTextureOverlay  );
	ImguiHelpers::ShowBool("RenderOpaque", m_RenderOpaque  );
	ImguiHelpers::ShowBool("RenderShadows", m_RenderShadows  );
	ImguiHelpers::ShowBool("OutputGLErrors", m_OutputGLErrors  );
	ImguiHelpers::ShowBool("DisplayDebugModels", m_DisplayDebugModels  );
	ImguiHelpers::ShowBool("PickingEnabled", m_PickingEnabled  );
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
