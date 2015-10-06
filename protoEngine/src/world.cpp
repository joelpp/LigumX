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
	normalized = glm::trunc(normalized);
	return normalized * m_sectorSize;
}

Sector* World::SectorFromXY(Coord2 longLat){
	Coord2 sp = sectorPosFromXY(longLat); 
	PRINTVEC2(sp);
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
