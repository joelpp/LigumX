#include "stdafx.h"
#include "DisplayOptions.h"
#include <cstddef>

#pragma region  CLASS_SOURCE DisplayOptions
#include "DisplayOptions.h"
#include <cstddef>
const ClassPropertyData DisplayOptions::g_Properties[] = 
{
{ "DrawTerrain", offsetof(DisplayOptions, m_DrawTerrain), 0, LXType_bool, false, LXType_None,  }, 
{ "DrawSky", offsetof(DisplayOptions, m_DrawSky), 0, LXType_bool, false, LXType_None,  }, 
{ "WireframeRendering", offsetof(DisplayOptions, m_WireframeRendering), 0, LXType_bool, false, LXType_None,  }, 
{ "UseLighting", offsetof(DisplayOptions, m_UseLighting), 0, LXType_bool, false, LXType_None,  }, 
{ "ShowNormals", offsetof(DisplayOptions, m_ShowNormals), 0, LXType_bool, false, LXType_None,  }, 
{ "ShowSpecular", offsetof(DisplayOptions, m_ShowSpecular), 0, LXType_bool, false, LXType_None,  }, 
{ "ShowDiffuse", offsetof(DisplayOptions, m_ShowDiffuse), 0, LXType_bool, false, LXType_None,  }, 
{ "ShowAmbient", offsetof(DisplayOptions, m_ShowAmbient), 0, LXType_bool, false, LXType_None,  }, 
{ "ShowFPS", offsetof(DisplayOptions, m_ShowFPS), 0, LXType_bool, false, LXType_None,  }, 
{ "ShowDepth", offsetof(DisplayOptions, m_ShowDepth), 0, LXType_bool, false, LXType_None,  }, 
{ "LinearizeDepth", offsetof(DisplayOptions, m_LinearizeDepth), 0, LXType_bool, false, LXType_None,  }, 
{ "BlinnPhongShading", offsetof(DisplayOptions, m_BlinnPhongShading), 0, LXType_bool, false, LXType_None,  }, 
{ "RenderTextureOverlay", offsetof(DisplayOptions, m_RenderTextureOverlay), 0, LXType_bool, false, LXType_None,  }, 
{ "RenderOpaque", offsetof(DisplayOptions, m_RenderOpaque), 0, LXType_bool, false, LXType_None,  }, 
};

#pragma endregion  CLASS_SOURCE DisplayOptions
