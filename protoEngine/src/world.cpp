#include "world.h"
#include "sector.h"

using namespace std;
using namespace glm;



World::World()
{
}

World::World(float sectorSize)
{
	m_sectorSize = sectorSize;
	m_invSectorSize = 1.f / sectorSize;
}

Sector* World::createSector(Coord2 pos){
	Sector* c = new Sector(pos, m_sectorSize);

    m_sectors.emplace(pos, c);

    return c;
}

Coord2 World::Normalized(Coord2 UnNormalized){
	return UnNormalized * m_invSectorSize;
}

Coord2 World::sectorPosFromXY(Coord2 longLat){
	Coord2 normalized = Normalized(longLat);
	normalized = glm::floor(normalized);

	return normalized * m_sectorSize;
}

Sector* World::GetOrCreateSectorAtXY(Coord2 longLat, SectorList* newSectors){
	Sector* sector;
	try{
		sector = m_sectors.at(longLat);
	}
	catch(...){
		PRINT("Creating sector at ");
		PRINTVEC2(longLat);
		sector = new Sector(longLat, m_sectorSize);

    	m_sectors.emplace(longLat, sector);

    	if (newSectors)
    	{
			newSectors->push_back(sector);
		}
	}

	return sector;
}

Sector* World::GetOrCreateSectorContainingXY(Coord2 longLat){
	Coord2 sp = sectorPosFromXY(longLat); 
	Sector* sector;
	try{
		sector = m_sectors.at(sp);
	}
	catch(...){
		sector = new Sector(sp, m_sectorSize);

    	m_sectors.emplace(sp, sector);
	}

	return sector;
}

SectorList* World::loadSectorsAroundPoint(Coord2 point, int ringSize){
	SectorList* newSectors = new SectorList();
	Coord2 startingCoord = sectorPosFromXY(point - glm::vec2(ringSize * m_sectorSize));
	Coord2 runningCoord = startingCoord;;
	for (int i = 0; i < 2 * ringSize + 1; ++i )
	{
		runningCoord.x = startingCoord.x;

		for (int j = 0; j < 2 * ringSize + 1; ++j )
		{
			GetOrCreateSectorAtXY(runningCoord, newSectors);
			runningCoord.x += m_sectorSize;
		}
		runningCoord.y += m_sectorSize;
	}
	return newSectors;
}



