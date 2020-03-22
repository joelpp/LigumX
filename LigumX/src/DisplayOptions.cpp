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
	serializer.SerializeBool(g_Properties[PIDX_UseSkyLighting], m_UseSkyLighting);
	serializer.SerializeBool(g_Properties[PIDX_RenderTerrain], m_RenderTerrain);
	serializer.SerializeBool(g_Properties[PIDX_DrawSky], m_DrawSky);
	serializer.SerializeBool(g_Properties[PIDX_WireframeRendering], m_WireframeRendering);
	serializer.SerializeBool(g_Properties[PIDX_UseLighting], m_UseLighting);
	serializer.SerializeBool(g_Properties[PIDX_ShowNormals], m_ShowNormals);
	serializer.SerializeBool(g_Properties[PIDX_ShowUVs], m_ShowUVs);
	serializer.SerializeBool(g_Properties[PIDX_ShowSpecular], m_ShowSpecular);
	serializer.SerializeBool(g_Properties[PIDX_ShowDiffuse], m_ShowDiffuse);
	serializer.SerializeBool(g_Properties[PIDX_ShowAmbient], m_ShowAmbient);
	serializer.SerializeBool(g_Properties[PIDX_ShowFPS], m_ShowFPS);
	serializer.SerializeBool(g_Properties[PIDX_ShowDepth], m_ShowDepth);
	serializer.SerializeBool(g_Properties[PIDX_LinearizeDepth], m_LinearizeDepth);
	serializer.SerializeBool(g_Properties[PIDX_BlinnPhongShading], m_BlinnPhongShading);
	serializer.SerializeBool(g_Properties[PIDX_RenderTextureOverlay], m_RenderTextureOverlay);
	serializer.SerializeBool(g_Properties[PIDX_RenderOpaque], m_RenderOpaque);
	serializer.SerializeBool(g_Properties[PIDX_RenderShadows], m_RenderShadows);
	serializer.SerializeBool(g_Properties[PIDX_OutputGLErrors], m_OutputGLErrors);
	serializer.SerializeBool(g_Properties[PIDX_DisplayDebugModels], m_DisplayDebugModels);
	serializer.SerializeBool(g_Properties[PIDX_PickingEnabled], m_PickingEnabled);
}
bool DisplayOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DisplayOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UseSkyLighting], &m_UseSkyLighting  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RenderTerrain], &m_RenderTerrain  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DrawSky], &m_DrawSky  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_WireframeRendering], &m_WireframeRendering  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UseLighting], &m_UseLighting  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowNormals], &m_ShowNormals  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowUVs], &m_ShowUVs  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowSpecular], &m_ShowSpecular  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowDiffuse], &m_ShowDiffuse  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowAmbient], &m_ShowAmbient  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowFPS], &m_ShowFPS  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ShowDepth], &m_ShowDepth  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_LinearizeDepth], &m_LinearizeDepth  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_BlinnPhongShading], &m_BlinnPhongShading  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RenderTextureOverlay], &m_RenderTextureOverlay  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RenderOpaque], &m_RenderOpaque  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_RenderShadows], &m_RenderShadows  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_OutputGLErrors], &m_OutputGLErrors  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_DisplayDebugModels], &m_DisplayDebugModels  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_PickingEnabled], &m_PickingEnabled  );
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
