#include "stdafx.h"
#include <cstddef>
#include "DisplayOptions.h"
#include "Serializer.h"
#include <string>
#include <fstream>

#pragma region  CLASS_SOURCE DisplayOptions
#include "DisplayOptions.h"
#include <cstddef>
const ClassPropertyData DisplayOptions::g_Properties[] = 
{
{ "UseSkyLighting", offsetof(DisplayOptions, m_UseSkyLighting), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DrawTerrain", offsetof(DisplayOptions, m_DrawTerrain), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "DrawSky", offsetof(DisplayOptions, m_DrawSky), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "WireframeRendering", offsetof(DisplayOptions, m_WireframeRendering), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "UseLighting", offsetof(DisplayOptions, m_UseLighting), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowNormals", offsetof(DisplayOptions, m_ShowNormals), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowSpecular", offsetof(DisplayOptions, m_ShowSpecular), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowDiffuse", offsetof(DisplayOptions, m_ShowDiffuse), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowAmbient", offsetof(DisplayOptions, m_ShowAmbient), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowFPS", offsetof(DisplayOptions, m_ShowFPS), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "ShowDepth", offsetof(DisplayOptions, m_ShowDepth), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "LinearizeDepth", offsetof(DisplayOptions, m_LinearizeDepth), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "BlinnPhongShading", offsetof(DisplayOptions, m_BlinnPhongShading), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderTextureOverlay", offsetof(DisplayOptions, m_RenderTextureOverlay), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderOpaque", offsetof(DisplayOptions, m_RenderOpaque), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "RenderShadows", offsetof(DisplayOptions, m_RenderShadows), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "OutputGLErrors", offsetof(DisplayOptions, m_OutputGLErrors), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};
void DisplayOptions::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "DisplayOptions.LXobj";

			int fileMask = writing ? std::ios::out : std::ios::in;
			std::fstream objectStream(basePath + fileName, fileMask);

			if (objectStream.is_open())
			{
				if (writing)
				{
					objectStream << "UseSkyLighting" << std::endl;
					objectStream << m_UseSkyLighting << std::endl;
					objectStream << "DrawTerrain" << std::endl;
					objectStream << m_DrawTerrain << std::endl;
					objectStream << "DrawSky" << std::endl;
					objectStream << m_DrawSky << std::endl;
					objectStream << "WireframeRendering" << std::endl;
					objectStream << m_WireframeRendering << std::endl;
					objectStream << "UseLighting" << std::endl;
					objectStream << m_UseLighting << std::endl;
					objectStream << "ShowNormals" << std::endl;
					objectStream << m_ShowNormals << std::endl;
					objectStream << "ShowSpecular" << std::endl;
					objectStream << m_ShowSpecular << std::endl;
					objectStream << "ShowDiffuse" << std::endl;
					objectStream << m_ShowDiffuse << std::endl;
					objectStream << "ShowAmbient" << std::endl;
					objectStream << m_ShowAmbient << std::endl;
					objectStream << "ShowFPS" << std::endl;
					objectStream << m_ShowFPS << std::endl;
					objectStream << "ShowDepth" << std::endl;
					objectStream << m_ShowDepth << std::endl;
					objectStream << "LinearizeDepth" << std::endl;
					objectStream << m_LinearizeDepth << std::endl;
					objectStream << "BlinnPhongShading" << std::endl;
					objectStream << m_BlinnPhongShading << std::endl;
					objectStream << "RenderTextureOverlay" << std::endl;
					objectStream << m_RenderTextureOverlay << std::endl;
					objectStream << "RenderOpaque" << std::endl;
					objectStream << m_RenderOpaque << std::endl;
					objectStream << "RenderShadows" << std::endl;
					objectStream << m_RenderShadows << std::endl;
					objectStream << "OutputGLErrors" << std::endl;
					objectStream << m_OutputGLErrors << std::endl;
}
				else
				{

					int i = 0;
					std::string line;
					while (std::getline(objectStream, line))
					{
						if (line == "")
						{
							continue;
						}

						bool found = false;
						for (int i = 0; i < g_PropertyCount; ++i)
						{
							const ClassPropertyData& data = g_Properties[i];
							if (line == data.m_Name)
							{
								bool value;
								objectStream >> value;

								char* propertyPtr = (char*)this + data.m_Offset;
								*((bool*)propertyPtr) = value;

								found = true;
								break;
							}
						}
					}

				}
			}
		}

#pragma endregion  CLASS_SOURCE DisplayOptions

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
//			objectStream << m_DrawTerrain << std::endl;
//			objectStream << m_DrawSky << std::endl;
//		}
//		else
//		{
//			objectStream >> m_UseSkyLighting;
//			objectStream >> m_DrawTerrain;
//			objectStream >> m_DrawSky;
//		}
//	}
//
//
//}
