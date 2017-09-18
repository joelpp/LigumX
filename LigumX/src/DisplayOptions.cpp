#include "stdafx.h"
#include "DisplayOptions.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions
#include "DisplayOptions.h"
#include <cstddef>
const ClassPropertyData DisplayOptions::g_DisplayOptionsProperties[] = 
{
{ "DrawTerrain", offsetof(DisplayOptions, m_DrawTerrain), 1 },
{ "DrawSky", offsetof(DisplayOptions, m_DrawSky), 1 },
{ "WireframeRendering", offsetof(DisplayOptions, m_WireframeRendering), 1 },
{ "UseLighting", offsetof(DisplayOptions, m_UseLighting), 1 },
{ "ShowNormals", offsetof(DisplayOptions, m_ShowNormals), 1 },
{ "ShowSpecular", offsetof(DisplayOptions, m_ShowSpecular), 1 },
{ "ShowDiffuse", offsetof(DisplayOptions, m_ShowDiffuse), 1 },
{ "ShowAmbient", offsetof(DisplayOptions, m_ShowAmbient), 1 },
{ "ShowFPS", offsetof(DisplayOptions, m_ShowFPS), 1 },
{ "ShowDepth", offsetof(DisplayOptions, m_ShowDepth), 1 },
{ "LinearizeDepth", offsetof(DisplayOptions, m_LinearizeDepth), 1 },
{ "BlinnPhongShading", offsetof(DisplayOptions, m_BlinnPhongShading), 1 },
};

#pragma endregion  CLASS_SOURCE DisplayOptions
