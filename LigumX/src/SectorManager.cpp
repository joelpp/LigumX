

#include "RenderDataManager.h"
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
#include "OSMDataProcessor.h"
#include "OSMElementTypeDataStore.h"
#include "tinyxml2\tinyxml2.h"
#include "EngineSettings.h"
#include "VectorUtils.h"

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

void SectorManager::Reset()
{
	for (auto nodeIt = m_AllNodes.begin(); nodeIt != m_AllNodes.end(); ++nodeIt)
	{
		Node* node = nodeIt->second;

		delete node;
	}
	m_AllNodes.clear();
	m_AllNodesPtr.clear();

	for (auto wayIt = m_AllWays.begin(); wayIt != m_AllWays.end(); ++wayIt)
	{
		Way* way = wayIt->second;

		delete way;
	}
	m_AllWays.clear();
	m_AllWaysPtr.clear();
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
	lxAssert0(); // dead code
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
											 //if ( /*(manhattanDistance <= 0) &&*/ (sector->createHeightfield()) )
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

void SectorManager::LoadSectors(int loadingRingSize, const glm::vec2& earthStartCoords, const glm::vec2& worldStartCoords, const glm::ivec2& normalizedSectorIndex)
{
	World* world = LigumX::GetInstance().GetWorld();
	lxAssert(world);

	bool loadOSMData = g_EngineSettings->GetLoadOSMData();
	glm::vec2 earthExtent = glm::vec2(g_EngineSettings->GetExtent());

	int numSectorsPerSide = 2 * loadingRingSize - 1;
	int offset = loadingRingSize - 1;

	for (int i = 0; i < numSectorsPerSide; ++i)
	{
		for (int j = 0; j < numSectorsPerSide; ++j)
		{
			glm::ivec2 offsets = glm::ivec2(i - offset, j - offset);

			
			LXString startCoordsStringX = StringUtils::ToStringPrecise(earthStartCoords.x, 7);
			LXString startCoordsStringY = StringUtils::ToStringPrecise(earthStartCoords.y, 7);
			startCoordsStringX = StringUtils::RemoveSubstrings(startCoordsStringX, ".");
			startCoordsStringY = StringUtils::RemoveSubstrings(startCoordsStringY, ".");

			glm::ivec2 earthIntStartCoords = glm::ivec2(std::atoi(startCoordsStringX.c_str()), std::atoi(startCoordsStringY.c_str()));
			
			glm::ivec2 requestedSectorIndex = normalizedSectorIndex + glm::ivec2(offsets);

			std::string fileName = startCoordsStringX + "x" + startCoordsStringY;
			CurlRequest request = CurlRequest(earthStartCoords - earthExtent * (glm::vec2) offsets, earthExtent, false, fileName);
			request.SetSectorIndex(requestedSectorIndex);

			Sector* requestSector = world->GetSectorByIndex(requestedSectorIndex);


			if (!requestSector)
			{
				requestSector = CreateSector(requestedSectorIndex);
			}

			requestSector->SetQuantizedPosition(earthIntStartCoords);

			request.SetSector(requestSector);

			if (requestSector->GetDataLoaded())
			{
				continue;
			}

			if (loadOSMData)
			{
				request.Initialize();
				request.Start();
				request.End();

				if (request.GetResult() != "")
				{
					LoadRequest(&request, SectorData::EOSMDataType::MAP);

					g_OSMDataProcessor->ProcessSector(requestSector);

					requestSector->SetDataLoaded(true);
				}

			}

		}
	}
}


void SectorManager::LoadRequest(CurlRequest* request, SectorData::EOSMDataType dataType)
{
	World* world = LigumX::GetInstance().GetWorld();
	lxAssert(world);

	tinyxml2::XMLDocument doc;

	const std::string& path = request->GetFilename();

	doc.LoadFile(path.c_str());

	tinyxml2::XMLNode* docRoot = doc.FirstChild()->NextSibling();

	std::fstream logFile("C:/temp/log.txt", std::fstream::out);
	glm::vec2 gpsScale2 = glm::vec2(g_EngineSettings->GetOSMQuantizationScale(), g_EngineSettings->GetOSMQuantizationScale());
	glm::highp_ivec2 sectorQuantizedCoordinate = glm::highp_ivec2(request->GetSector()->GetQuantizedPosition());
	glm::vec2 extent2 = glm::vec2(g_EngineSettings->GetExtent(), g_EngineSettings->GetExtent());
	glm::vec2 sectorQuantizedEarthSize = extent2 * gpsScale2;
	glm::vec2 worldScale2 = glm::vec2(g_EngineSettings->GetWorldScale(), g_EngineSettings->GetWorldScale());

	glm::vec2 sectorWorldPosition = glm::vec2(request->GetSectorIndex()) * worldScale2;

	for (tinyxml2::XMLNode* child = docRoot->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		std::string childValue = std::string(child->Value());
		if (childValue == "bound")
		{

		}

		else if (childValue == "node")
		{
			const std::string& id = child->ToElement()->Attribute("id");

			std::vector<std::string> debugNodeID = 
			{
				"3163852168",
				//"1957579363",
				//"1957579303",
				//"1957579235",
				//"1957579147",
				//"1957579118",
				//"1957579060",
				//"1957578945",
				//"1957578841",
				//"1957578813",
				//"1957578797",
				//"1957578704",
				//"1957578527",
				//"1957578270",
				//"1957710804",
				//"1957577978",
				//"1957577918",
				//"1957577826",
				//"1957577576",
				//"1957577446",
				//"1957577248",
				//"1957576879",
				//"1957576710",
				//"1957576657",
				//"1957576515",
				//"1957576373",
				//"1957576279",
				//"1957576136",
				//"1957575999",
				//"1957575876",
			};
			bool isDebugNode = VectorUtils::Contains(debugNodeID, id);
			if (!isDebugNode)
			{
				//continue;
			}

			if (m_AllNodes.find(id) != m_AllNodes.end())
			{
				continue;
			}

			float longitude = 0.f;// (float)atof();
			float latitude = 0.f;//(float)atof(child->ToElement()->Attribute("lat"));

			Node* node = new Node(id, longitude, latitude);
			node->SetOSMId(StringUtils::ToInt64(id));

			std::string longitudeString(child->ToElement()->Attribute("lon"));
			std::string latitudeString(child->ToElement()->Attribute("lat"));

			if (isDebugNode)
			{
				//latitudeString = "48.2312192";
			}

			std::string quantizedLongitudeString = StringUtils::RemoveSubstrings(longitudeString, ".");
			std::string quantizedLatitudeString = StringUtils::RemoveSubstrings(latitudeString, ".");

			lxInt64 iLong = StringUtils::ToInt64(quantizedLongitudeString.c_str());
			lxInt64 iLat = StringUtils::ToInt64(quantizedLatitudeString.c_str());

			glm::highp_ivec2 nodeSectorQuantizedCoordinates = glm::highp_ivec2(sectorQuantizedCoordinate);

			glm::highp_ivec2 longLat64 = glm::highp_ivec2(iLong, iLat);
			node->SetHighPrecisionEarthCoordinates(longLat64);

			glm::highp_ivec2 iSector = longLat64 - nodeSectorQuantizedCoordinates;

			glm::ivec2 sectorIndex(0, 0);
			//if (iSector.x < 0)
			//{
			//	int mul = (int)(iSector.x / sectorQuantizedEarthSize.x) - 1;
			//	float ceil = abs(mul * sectorQuantizedEarthSize.x);
			//	iSector.x = ceil - abs(iSector.x);
			//	sectorIndex.x = mul;
			//}

			//if (iSector.y < 0)
			//{
			//	int mul = (int)(iSector.y / sectorQuantizedEarthSize.y - 1);
			//	float ceil = abs(mul * sectorQuantizedEarthSize.y);
			//	iSector.y = ceil - iSector.y;
			//	sectorIndex.y = mul;
			//}

			glm::vec2 f01Sector = glm::vec2(iSector) / sectorQuantizedEarthSize;

			glm::vec2 worldPos = sectorWorldPosition + f01Sector * worldScale2;

			node->SetQuantizedSectorPosition(glm::ivec2(iSector));
			node->SetSectorRelativePosition(f01Sector);
			node->SetSectorIndex(sectorIndex);

			for (tinyxml2::XMLNode* tag = child->FirstChildElement(); tag != NULL; tag = tag->NextSiblingElement())
			{
				const std::string& key = tag->ToElement()->Attribute("k");
				const std::string& value = tag->ToElement()->Attribute("v");
				node->addTag(key, value);
			}

			Sector* sector = world->GetSectorByIndex(request->GetSectorIndex());

			if (!sector)
			{
				sector = CreateSector(request->GetSectorIndex());
			}

			float worldScale = g_EngineSettings->GetWorldScale();

			glm::vec2 posInSector = worldPos / worldScale;

			glm::vec2 normalizedPosInSector = glm::fract(posInSector);

			float sampledHeight = 0.f;// sector->SampleHeight(normalizedPosInSector);

			node->elevation = sampledHeight;
			node->SetWorldPosition(glm::vec3(worldPos, sampledHeight));

			//if (sector && sectorIndex == request->GetSectorIndex() && !sector->GetDataLoaded())
			//{
			//	//sector->InitializeFromRequest(request);
			//	std::string filename = path;
			//	sector->SetOSMFilename(filename);
			//	request->SetSector(sector);
			//}

			std::unordered_map<std::string, Node*>& nodes = sector->m_Data->nodes;

			nodes.emplace(id, node);

			m_AllNodes.emplace(id, node);

			lxInt64 intID = StringUtils::ToInt64(id);

			if (intID == 2147483647)
			{
				std::string newid = id.substr(3, id.size());
				intID = StringUtils::ToInt64(newid);
			}

			sector->m_Data->m_AllNodesPtr[intID] = node;

			m_AllNodesPtr[intID] = node;

		}

		else if (childValue == "way")
		{
			const std::string& id = child->ToElement()->Attribute("id");

			if (m_AllWays.find(id) != m_AllWays.end())
			{
				continue;
			}

			std::vector<std::string> debugUniqueWays = { "185200807" };
			bool isDebugWay = false;
			if ((debugUniqueWays.size() > 0) && (VectorUtils::Contains(debugUniqueWays, id)))
			{
				isDebugWay = true;
			}

			Way* way = new Way(id);
			way->SetOSMId(StringUtils::ToInt64(id.c_str()));

			for (tinyxml2::XMLNode* way_child = child->FirstChildElement(); way_child != NULL; way_child = way_child->NextSiblingElement())
			{
				if (strcmp(way_child->Value(), "nd") == 0)
				{
					const std::string& ref = way_child->ToElement()->Attribute("ref");

					Node* node = m_AllNodes[ref];
					way->AddNode(node);

					node->AddWay(way);
				}
				else if (strcmp(way_child->Value(), "tag") == 0)
				{
					const std::string& key = way_child->ToElement()->Attribute("k");
					const std::string& value = way_child->ToElement()->Attribute("v");

					way->AddTo_AllTags(key + " " + value + " ");
					way->addTag(key, value);

					logFile << key << " " << value << "\n";
				}
			}

			if (way->hasTag("name"))
			{
				way->SetName(way->getValue("name"));
			}
			else
			{
				LXString name = lxFormat("Unnamed (%s)", way->GetAllTags().c_str());
				way->SetName(name);
			}

			if (dataType == SectorData::CONTOUR)
			{
				float elevation = (float)atof(way->tags["ele"].c_str()) / 15000 + 0.000001f;
				for (auto it = way->GetNodes().begin(); it != way->GetNodes().end(); ++it)
				{
					Node* node = *it;
					node->elevation = 0.f;// elevation;
				}
			}

			if (way->GetOSMElementType() == OSMElementType_Unknown)
			{
				OSMElementType elementType = g_OSMElementTypeDataStore->GetWayType(way);

				if (elementType != OSMElementType_Unknown)
				{
					way->SetOSMElementType(elementType);

					float nodeHeight = 0.1f;
					if (way->IsPark())
					{
						nodeHeight *= 2.f;
					}
					else if (way->IsWater())
					{
						nodeHeight *= 3.f;
					}
					else if (way->IsRoad())
					{
						nodeHeight *= 4.f;
					}

					for (int n = 0; n < way->GetNodes().size(); ++n)
					{
						Node* node = way->GetNodes()[n];
						node->GetWorldPosition().z = nodeHeight;
					}


				}
			}
			std::unordered_map<std::string, Way*>& ways = request->GetSector()->m_Data->ways;
			ways.emplace(id, way);

			m_AllWays.emplace(id, way);

			lxInt64 intID = StringUtils::ToInt64(id);
			way->SetIndexInSector((int)request->GetSector()->m_Data->m_AllWaysPtr.size());
			request->GetSector()->m_Data->m_AllWaysPtr[intID] = way;
			m_AllWaysPtr[intID] = way;
		}

	}
	logFile.close();

}

void SectorManager::AddSector(Sector* sector)
{
	LigumX::GetInstance().GetWorld()->GetSectors().push_back(sector);
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


Node* SectorManager::GetClosestNode(glm::vec2 wsPosition, bool searchOnlyWithinSector)
{
	float longestDistance = 99999.f;

	Node* toReturn = nullptr;

	std::map<lxInt64, Node*>* allNodesPtr = &m_AllNodesPtr;

	if (searchOnlyWithinSector)
	{
		glm::ivec2 normalizedSectorIndex = Sector::GetNormalizedSectorIndex(wsPosition);

		World *world = LigumX::GetInstance().GetWorld();
		lxAssert(world);

		Sector* clickedSector = world->GetSectorByIndex(normalizedSectorIndex);

		if (clickedSector)
		{
			allNodesPtr = &(clickedSector->m_Data->m_AllNodesPtr);
		}
	}

	for (auto it = allNodesPtr->begin(); it != allNodesPtr->end(); ++it)
	{
		Node* node = it->second;
		glm::vec2 nodePos = glm::vec2(node->GetWorldPosition());
		float length = glm::length(nodePos - wsPosition);

		if (length < longestDistance)
		{
			longestDistance = length;
			toReturn = node;
		}
	}

	return toReturn;
}

