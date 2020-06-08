

#include "SectorMath.h"


Coord2 SectorMath::Normalized(Coord2 UnNormalized, float sectorSize)
{
	return UnNormalized / sectorSize;
}

Coord2 SectorMath::sectorPosFromXY(Coord2 longLat, float sectorSize)
{
	Coord2 normalized = Normalized(longLat, 1.f / sectorSize);
	normalized = glm::floor(normalized);

	return normalized * sectorSize;
}