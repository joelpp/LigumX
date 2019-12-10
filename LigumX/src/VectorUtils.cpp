#include "VectorUtils.h"

bool VectorUtils::Contains(const std::vector<std::string>& v, const std::string& val)
{
	return std::find(v.begin(), v.end(), val) != v.end();
}