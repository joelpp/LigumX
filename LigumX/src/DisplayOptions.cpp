#include "stdafx.h"
#include "DisplayOptions.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions
#include "DisplayOptions.h"
#include <cstddef>
const ClassPropertyData DisplayOptions::g_DisplayOptionsProperties[] = 
{
{ "DrawTerrain", offsetof(DisplayOptions, m_DrawTerrain), 0, LXType_bool, false,  }, 
{ "DrawSky", offsetof(DisplayOptions, m_DrawSky), 0, LXType_bool, false,  }, 
{ "WireframeRendering", offsetof(DisplayOptions, m_WireframeRendering), 0, LXType_bool, false,  }, 
{ "UseLighting", offsetof(DisplayOptions, m_UseLighting), 0, LXType_bool, false,  }, 
{ "ShowNormals", offsetof(DisplayOptions, m_ShowNormals), 0, LXType_bool, false,  }, 
{ "ShowSpecular", offsetof(DisplayOptions, m_ShowSpecular), 0, LXType_bool, false,  }, 
{ "ShowDiffuse", offsetof(DisplayOptions, m_ShowDiffuse), 0, LXType_bool, false,  }, 
{ "ShowAmbient", offsetof(DisplayOptions, m_ShowAmbient), 0, LXType_bool, false,  }, 
{ "ShowFPS", offsetof(DisplayOptions, m_ShowFPS), 0, LXType_bool, false,  }, 
{ "ShowDepth", offsetof(DisplayOptions, m_ShowDepth), 0, LXType_bool, false,  }, 
{ "LinearizeDepth", offsetof(DisplayOptions, m_LinearizeDepth), 0, LXType_bool, false,  }, 
{ "BlinnPhongShading", offsetof(DisplayOptions, m_BlinnPhongShading), 0, LXType_bool, false,  }, 
{ "RenderTextureOverlay", offsetof(DisplayOptions, m_RenderTextureOverlay), 0, LXType_bool, false,  }, 
};

#pragma endregion  CLASS_SOURCE DisplayOptions
