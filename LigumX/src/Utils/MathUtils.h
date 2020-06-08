#pragma once

#include "glm\glm.hpp"

namespace MathUtils
{


	bool FuzzyEquals(float a, float b, float tolerance);

	bool FuzzyEquals(glm::vec2 a, glm::vec2 b, float tolerance);

}