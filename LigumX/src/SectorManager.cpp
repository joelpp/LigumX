#include "stdafx.h"

#include "SectorManager.h"
#include "Sector.h"

SectorManager::SectorManager()
{
}

SectorManager::SectorManager(float sectorSize)
{
	m_sectorSize = sectorSize;
	bLoadNewSectors = true;
}

int SectorManager::getSectorIndex(float x, float y)
{
	int startX = -180;
	int startY = -90;
	/*int startX = 0;
	int startY = 0;*/

	int xSize = 360;
	int ySize = 180;

	float a = x / m_sectorSize;
	float b = y / m_sectorSize;

	int sectorX = floor(a);
	int sectorY = floor(b);

	int numSectorsX = xSize / m_sectorSize;
	int numSectorsY = ySize / m_sectorSize;

	sectorX -= startX;
	sectorY -= startY;

	return sectorX * numSectorsY + sectorY;
}

int SectorManager::IDFromPos(glm::vec2 pos)
{	
	//IntPair coords = intPairFromVec2(pos);
	return getSectorIndex(pos.x, pos.y);
}

glm::vec2 SectorManager::posFromID(int ID)
{	
	int startX = -180;
	int startY = -90;
	//int startX = 0;
	//int startY = 0;

	int xSize = 360;
	int ySize = 180;

	int numSectorsY = ySize / m_sectorSize;

	int sectorY = ID % numSectorsY;

	// integer division on purpose
	int sectorX = (ID - sectorY) / numSectorsY;

	return m_sectorSize * glm::vec2(startX + sectorX, startY + sectorY);
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
	int ID = IDFromPos(longLat);
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
											//sector->loadData(SectorData::CONTOUR);
											sector->m_initializationLevel = Sector::ContourLoaded;
											break;

		case Sector::ContourLoaded:			
											//sector->loadData(SectorData::MAP);
											sector->m_initializationLevel = Sector::DataLoaded;
											break;

		case Sector::DataLoaded:			
											 if ( (manhattanDistance <= 0) && (sector->createHeightfield()) )
											{
												sector->m_initializationLevel = Sector::HeightfieldGenerated;
			  								}
			  							 	else
			  							 	{
												 sector->m_initializationLevel = Sector::HeightfieldGenerating;
			  							 	}
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

			if (bLoadNewSectors && initialization && (sector->m_initializationLevel < Sector::FullyInitialized))
			{
				keepInitializing(sector, manhattanDistance);
			}

			runningCoord.x += m_sectorSize;
		}	
		runningCoord.y += m_sectorSize;
	}
	return newSectors;
}