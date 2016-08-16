#include "stdafx.h"

#include "SectorManager.h"
#include "Sector.h"

SectorManager::SectorManager()
{
}

SectorManager::SectorManager(float sectorSize)
{
	m_sectorSize = sectorSize;
}

long int SectorManager::getSectorIndex(int x, int y)
{
	//lol i broke everything
	return y * (int)(1.f / 0.01) * 360 + x;
}

int SectorManager::IDFromPos(glm::vec2 pos)
{	
	IntPair coords = intPairFromVec2(pos);
	return getSectorIndex(coords.first, coords.second);
}

glm::vec2 SectorManager::posFromID(int ID)
{	
	int width = (int)((1.f / m_sectorSize) * 360);
	int xPos = (ID % width); 
	int yPos = (ID - xPos) / width;
	// xPos = -60;
	// yPos = 52;

	return glm::vec2((xPos / 100.f) - 180.f, (yPos / 100.f) - 45.f);
}

IntPair SectorManager::intPairFromVec2(glm::vec2 pos)
{
	pos.x = roundf( (pos.x + 180) * 100 );
	pos.y = roundf( (pos.y + 45) * 100 );

	return IntPair((int)pos.x, (int)pos.y);
}

glm::vec2 SectorManager::posFromVec2(glm::vec2 pos)
{
	pos.x = roundf((pos.x+0.5)*100) / 100.f;
	pos.y = roundf((pos.y+0.5)*100) / 100.f;

	return pos;
}

Sector* SectorManager::createSector(Coord2 pos)
{

	glm::vec2 sectorCoords = posFromVec2(pos);
	int newSectorID = getSectorIndex((int) sectorCoords.x, (int) sectorCoords.y);
	Sector* c = new Sector(sectorCoords, m_sectorSize, newSectorID);

    m_sectors.emplace(newSectorID, c);

    return c;
}
Sector* SectorManager::createSector(int ID)
{
	glm::vec2 sectorCoords = posFromID(ID);
	Sector* c = new Sector( sectorCoords, m_sectorSize, ID);

    m_sectors.emplace(ID, c);

    return c;
}
// Coord2 World::Normalized(Coord2 UnNormalized){
// 	return UnNormalized * m_invSectorSize;
// }

// Coord2 World::sectorPosFromXY(Coord2 longLat){
// 	Coord2 normalized = Normalized(longLat);
// 	normalized = glm::floor(normalized);

// 	return normalized * m_sectorSize;
// }

Sector* SectorManager::sectorContaining(Coord2 longLat){
	Sector* sector;
	long int ID = IDFromPos(longLat);
	auto found = m_sectors.find(ID);

	if (found != m_sectors.end())
	{
		sector = m_sectors.at(ID);
	}
	else
	{
		// PRINT("Creating sector at ");
		// PRINTVEC2(longLat);
		sector = createSector(ID);
    	m_sectors.emplace(ID, sector);
		PRINTINT(m_sectors.size());

	}


	return sector;
}

int minHeightfieldDistance = 1;
void SectorManager::keepInitializing(Sector* sector, int manhattanDistance)
{
	switch(sector->m_initializationLevel)
	{
		case Sector::Uninitialized:			
											sector->loadData(SectorData::CONTOUR);
											sector->m_initializationLevel = Sector::ContourLoaded;
											break;

		case Sector::ContourLoaded:			
											sector->loadData(SectorData::MAP);
											sector->m_initializationLevel = Sector::DataLoaded;
											break;

		case Sector::DataLoaded:			
											 if ( (manhattanDistance <= minHeightfieldDistance) && (sector->createHeightfield()) )
											{
												sector->m_initializationLevel = Sector::HeightfieldGenerated;
			  								}
			  							// 	else
			  							// 	{
												// sector->m_initializationLevel = Sector::HeightfieldGenerating;
			  							// 	}
			  								break;

		case Sector::HeightfieldGenerating:	
											// if ( (manhattanDistance <= minHeightfieldDistance) && (sector->createHeightfield()) )
											{
												sector->m_initializationLevel = Sector::HeightfieldGenerated;
			  								}
			  								break;

		case Sector::HeightfieldGenerated:	
											sector->m_initializationLevel = Sector::FullyInitialized;
			  								break;

		case Sector::FullyInitialized:
											break;

	}
}


SectorList* SectorManager::sectorsAround(Coord2 point, int ringSize, bool initialization){
	SectorList* newSectors = new SectorList();
	Coord2 startingCoord = (point - glm::vec2(ringSize * m_sectorSize));
	Coord2 runningCoord = startingCoord;
	for (int i = 0; i < 2 * ringSize + 1; ++i )
	{
		runningCoord.x = startingCoord.x;

		for (int j = 0; j < 2 * ringSize + 1; ++j )
		{

			int manhattanDistance = abs(i - ringSize) + abs(j - ringSize);
			Sector* sector = sectorContaining(runningCoord);
			newSectors->push_back(sector);

			if (initialization && (sector->m_initializationLevel < Sector::FullyInitialized))
			{
				keepInitializing(sector, manhattanDistance);
			}

			runningCoord.x += m_sectorSize;
		}	
		runningCoord.y += m_sectorSize;
	}
	return newSectors;
}