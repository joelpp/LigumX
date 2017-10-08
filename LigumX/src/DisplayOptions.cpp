#include "stdafx.h"
#include "DisplayOptions.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions
#include "DisplayOptions.h"
#include <cstddef>
const ClassPropertyData DisplayOptions::g_Properties[] = 
{
{ "DrawTerrain", offsetof(DisplayOptions, m_DrawTerrain), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "DrawSky", offsetof(DisplayOptions, m_DrawSky), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "WireframeRendering", offsetof(DisplayOptions, m_WireframeRendering), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "UseLighting", offsetof(DisplayOptions, m_UseLighting), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "ShowNormals", offsetof(DisplayOptions, m_ShowNormals), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "ShowSpecular", offsetof(DisplayOptions, m_ShowSpecular), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "ShowDiffuse", offsetof(DisplayOptions, m_ShowDiffuse), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "ShowAmbient", offsetof(DisplayOptions, m_ShowAmbient), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "ShowFPS", offsetof(DisplayOptions, m_ShowFPS), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "ShowDepth", offsetof(DisplayOptions, m_ShowDepth), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "LinearizeDepth", offsetof(DisplayOptions, m_LinearizeDepth), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "BlinnPhongShading", offsetof(DisplayOptions, m_BlinnPhongShading), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "RenderTextureOverlay", offsetof(DisplayOptions, m_RenderTextureOverlay), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "RenderOpaque", offsetof(DisplayOptions, m_RenderOpaque), 0, LXType_bool, false, LXType_None, 0,  }, 
{ "OutputGLErrors", offsetof(DisplayOptions, m_OutputGLErrors), 0, LXType_bool, false, LXType_None, 0,  }, 
};

#pragma endregion  CLASS_SOURCE DisplayOptions
