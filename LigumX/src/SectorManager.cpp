#include "stdafx.h"

#include "SectorManager.h"
#include "Sector.h"
#include "LigumX.h"
#include "CurlRequest.h"
#include "SectorData.h"
#include "Node.h"
#include "Way.h"
#include "World.h"
#include "EngineSettings.h"
#include "LXError.h"
#include "StringUtils.h"

SectorManager* g_SectorManager;


SectorManager::SectorManager()
{
}

SectorManager::SectorManager(float sectorSize)
{
	m_sectorSize = sectorSize;
    
    iStartX = -180;
    iStartY = -90;
    /*int startX = 0;
     int startY = 0;*/
    
    iSizeX = 360;
    iSizeY = 180;
    
    iNumSectorsX = iSizeX / (int) m_sectorSize;
    iNumSectorsY = iSizeY / (int) m_sectorSize;
}

int SectorManager::getSectorIndex(float x, float y)
{
	float a = x / m_sectorSize;
	float b = y / m_sectorSize;

	int sectorX = (int) floor(a);
	int sectorY = (int) floor(b);

	sectorX -= iStartX;
	sectorY -= iStartY;

	return sectorX * iNumSectorsY + sectorY;
}

int SectorManager::IDFromPos(glm::vec2 pos)
{	
	//IntPair coords = intPairFromVec2(pos);
	return getSectorIndex(pos.x, pos.y);
}

glm::vec2 SectorManager::posFromID(int ID)
{	

	int sectorY = ID % iNumSectorsY;

	// integer division on purpose
	int sectorX = (ID - sectorY) / iNumSectorsY;

	return m_sectorSize * glm::vec2(iStartX + sectorX, iStartY + sectorY);
}

IntPair SectorManager::intPairFromVec2(glm::vec2 pos)
{
	pos.x = roundf( (pos.x + 180) * 100 );
	pos.y = roundf( (pos.y + 45) * 100 );

	return IntPair((int)pos.x, (int)pos.y);
}

glm::vec2 SectorManager::posFromVec2(glm::vec2 pos)
{
	pos.x = roundf( (pos.x+0.5f) * 100 ) / 100.f;
	pos.y = roundf( (pos.y+0.5f) * 100 ) / 100.f;

	return pos;
}

//Sector* SectorManager::createSector(Coord2 pos)
//{
//
//	glm::vec2 sectorCoords = posFromVec2(pos);
//	int newSectorID = getSectorIndex(trunc(sectorCoords.x), trunc(sectorCoords.y));
//	Sector* c = new Sector(sectorCoords, m_sectorSize, newSectorID);
//
//	m_Sectors.emplace(newSectorID, c);
//
//    return c;
//}
//Sector* SectorManager::createSector(int ID)
//{
//	glm::vec2 sectorCoords = posFromID(ID);
//	Sector* c = new Sector( sectorCoords, m_sectorSize, ID);
//
//	m_Sectors.emplace(ID, c);
//
//    return c;
//}
// Coord2 World::Normalized(Coord2 UnNormalized){
// 	return UnNormalized * m_invSectorSize;
// }

// Coord2 World::sectorPosFromXY(Coord2 longLat){
// 	Coord2 normalized = Normalized(longLat);
// 	normalized = glm::floor(normalized);

// 	return normalized * m_sectorSize;
// }

Sector* SectorManager::sectorContaining(Coord2 longLat)
{
	Sector* sector;
	int ID = IDFromPos(longLat);
	auto found = m_Sectors.find(ID);

	if (found != m_Sectors.end())
	{
		sector = m_Sectors.at(ID);
	}
	else
	{
		// PRINT("Creating sector at ");
		// PRINTVEC2(longLat);
		sector = nullptr;// createSector(ID);
		lxAssert0();
		m_Sectors.emplace(ID, sector);
		PRINTINT(m_Sectors.size());

	}


	return sector;
}

Sector* SectorManager::getSector(int ID)
{
    Sector* sector;
	auto found = m_Sectors.find(ID);
    
    if (found != m_Sectors.end())
    {
        sector = m_Sectors.at(ID);
    }
    else
    {
        //sector = createSector(ID);
		lxAssert0();
		m_Sectors.emplace(ID, sector);
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
											 if ( /*(manhattanDistance <= 0) &&*/ (sector->createHeightfield()) )
											{
												sector->m_initializationLevel = Sector::HeightfieldGenerated;
			  								}
			  							 //	else
			  							 //	{
												 //sector->m_initializationLevel = Sector::HeightfieldGenerating;
			  							 //	}
			  								//break;

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

SectorList* SectorManager::sectorsAround(Coord2 point, int ringSize, bool initialization)
{
	SectorList* sectors = new SectorList();
    int ringCenterIndex = IDFromPos(point);
    
	for (int i = -ringSize; i <= ringSize; ++i )
	{
		for (int j = -ringSize; j <= ringSize; ++j )
		{
			int manhattanDistance = abs(i) + abs(j);
            int sectorIndex = ringCenterIndex + i + (iNumSectorsY * j);
            
            Sector* sector = getSector(sectorIndex);
			sectors->push_back(sector);

			if (initialization && (sector->m_initializationLevel < Sector::FullyInitialized))
			{
				keepInitializing(sector, manhattanDistance);
			}
		}
	}
	return sectors;
}

void SectorManager::ProcessXMLResult(tinyxml2::XMLNode* docRoot)
{
}

glm::ivec2 GetSectorIndexFromEarthLonLat(const glm::vec2& earthLonLat)
{
	glm::vec2 lonLat = earthLonLat;
	lonLat -= g_EngineSettings->GetStartLonLat();
	lonLat /= g_EngineSettings->GetExtent();

	glm::ivec2 sectorIndex = (glm::ivec2) glm::floor(lonLat);

	return sectorIndex;
}

void SectorManager::LoadRequest(CurlRequest* request, SectorData::EOSMDataType dataType)
{
	World *world = LigumX::GetInstance().world;
	tinyxml2::XMLDocument doc;

	const std::string& path = request->GetFilename();

	doc.LoadFile(path.c_str());

	tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();

	for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		std::string childValue = std::string(child->Value());
		if (childValue == "bound")
		{

		}

		else if (childValue == "node")
		{
			std::string id = child->ToElement()->FindAttribute("id")->Value();

			if (m_AllNodes.find(id) != m_AllNodes.end())
			{
				continue;
			}

			float longitude = (float)atof(child->ToElement()->FindAttribute("lon")->Value());
			float latitude = (float)atof(child->ToElement()->FindAttribute("lat")->Value());

			Node* node = new Node(id, longitude, latitude);

			for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement())
			{
				std::string key = tag->ToElement()->FindAttribute("k")->Value();
				std::string value = tag->ToElement()->FindAttribute("v")->Value();
				node->addTag(key, value);
			}

			glm::ivec2 sectorIndex = GetSectorIndexFromEarthLonLat(glm::vec2(longitude, latitude));

			Sector* sector = world->GetSectorByIndex(sectorIndex);

			if (!sector)
			{
				sector = CreateSector(sectorIndex);
			}

			if (sector && sectorIndex == request->GetSectorIndex() && !sector->GetDataLoaded())
			{
				//sector->InitializeFromRequest(request);
				std::string filename = std::string(request->GetFilename());
				sector->SetOSMFilename(filename);
				request->SetSector(sector);

			}

			std::unordered_map<std::string, Node*>& nodes = sector->m_Data->nodes;

			nodes.emplace(id, node);
			m_AllNodes.emplace(id, node);

			int intID = StringUtils::ToInt(id);
			m_AllNodesPtr[id] = node;
		}

		else if (childValue == "way")
		{
			std::string id = child->ToElement()->FindAttribute("id")->Value();

			if (m_AllWays.find(id) != m_AllWays.end())
			{
				continue;
			}

			Way* way = new Way(id);
			way->eType = OSMElement::NOT_IMPLEMENTED;

			Sector* sector = world->GetSectorByIndex(request->GetSectorIndex());

			for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement())
			{
				if (std::string(way_child->Value()).compare("nd") == 0)
				{
					std::string ref = way_child->ToElement()->FindAttribute("ref")->Value();
					way->AddNode(m_AllNodes[ref]);
				}
				else if (std::string(way_child->Value()).compare("tag") == 0)
				{
					std::string key = way_child->ToElement()->FindAttribute("k")->Value();
					std::string value = way_child->ToElement()->FindAttribute("v")->Value();
					way->addTag(key, value);

					OSMElement::ElementType _eType = OSMElement::typeFromStrings(key, value);
					if (_eType == OSMElement::NOT_IMPLEMENTED)
					{
						continue;
					}
					else
					{
						way->eType = _eType;
					}
				}
			}

			if (dataType == SectorData::CONTOUR)
			{
				float elevation = (float)atof(way->tags["ele"].c_str()) / 15000 + 0.000001f;
				for (auto it = way->GetNodes().begin(); it != way->GetNodes().end(); ++it)
				{
					Node* node = *it;
					node->elevation = elevation;
				}
			}


			std::unordered_map<std::string, Way*>& ways = sector->m_Data->ways;
			ways.emplace(id, way);

			m_AllWays.emplace(id, way);
		}
	}
}

void SectorManager::AddSector(Sector* sector)
{
	LigumX::GetInstance().world->GetSectors().push_back(sector);
}


Sector* SectorManager::CreateSector(CurlRequest* request)
{
	Sector* sector = new Sector(request);

	AddSector(sector);

	return sector;
}

Sector* SectorManager::CreateSector(const glm::ivec2& sectorIndex)
{
	Sector* sector = new Sector(sectorIndex);

	AddSector(sector);

	return sector;
}

glm::ivec2 SectorManager::EarthToQuantized(const glm::vec2& earthPosition)
{
	return (glm::ivec2) (earthPosition * (float) g_EngineSettings->GetOSMQuantizationScale());
}

int SectorManager::EarthToQuantized(const float& earthPosition)
{
	return (int)(earthPosition * g_EngineSettings->GetOSMQuantizationScale());
}

glm::vec2 SectorManager::OffsetIndexToWorldPosition(const glm::ivec2& sectorIndex)
{
	glm::vec2 worldPosition = (glm::vec2) sectorIndex * g_EngineSettings->GetWorldScale();

	return worldPosition;
}
