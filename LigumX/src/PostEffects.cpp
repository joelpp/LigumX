#include "stdafx.h"

#pragma region  CLASS_SOURCE PostEffects
#include "PostEffects.h"
#include <cstddef>
const ClassPropertyData PostEffects::g_Properties[] = 
{
{ "GammaCorrectionEnabled", offsetof(PostEffects, m_GammaCorrectionEnabled), 0, LXType_bool, false, LXType_None,  }, 
{ "GammaExponent", offsetof(PostEffects, m_GammaExponent), 0, LXType_float, false, LXType_None,  }, 
};

#pragma endregion  CLASS_SOURCE PostEffects
