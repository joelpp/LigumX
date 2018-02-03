#include "MathUtils.h"


bool MathUtils::FuzzyEquals(float a, float b, float tolerance)
{
	return fabs(a - b) < tolerance;
}

bool MathUtils::FuzzyEquals(glm::vec2 a, glm::vec2 b, float tolerance)
{
	return (fabs(a.x - b.x) < tolerance) && (fabs(a.y - b.y) < tolerance);
}
