#include "stdafx.h"

#pragma region  CLASS_SOURCE PostEffects
#include "PostEffects.h"
#include <cstddef>
const ClassPropertyData PostEffects::g_PostEffectsProperties[] = 
{
{ "GammaCorrectionEnabled", offsetof(PostEffects, m_GammaCorrectionEnabled), 1 },
{ "GammaExponent", offsetof(PostEffects, m_GammaExponent), 0 },
};

#pragma endregion  CLASS_SOURCE PostEffects
