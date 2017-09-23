#include "stdafx.h"

#pragma region  CLASS_SOURCE PostEffects
#include "PostEffects.h"
#include <cstddef>
const ClassPropertyData PostEffects::g_Properties[] = 
{
{ "GammaCorrectionEnabled", offsetof(PostEffects, m_GammaCorrectionEnabled), 0, LXType_bool, false,  }, 
{ "GammaExponent", offsetof(PostEffects, m_GammaExponent), 0, LXType_float, false,  }, 
};

#pragma endregion  CLASS_SOURCE PostEffects
