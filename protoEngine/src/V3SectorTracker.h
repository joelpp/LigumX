#pragma once

#include <vector>
#include "glm/glm.hpp"

typedef glm::vec2 Coord2;

class V3SectorTracker{



private:
	REGISTERCLASS(V3SectorTracker);


	void add(Coord* v3);
	std::vector<Coord2>* m_tracked; 
};