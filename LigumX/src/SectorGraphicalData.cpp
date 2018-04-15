#include "SectorGraphicalData.h"
#include "Mesh.h"
#include "Texture.h"
#include "EngineSettings.h"

#pragma region  CLASS_SOURCE SectorGraphicalData

#include "SectorGraphicalData.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData SectorGraphicalData::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(SectorGraphicalData, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", PIDX_Name, offsetof(SectorGraphicalData, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "NodesModel", PIDX_NodesModel, offsetof(SectorGraphicalData, m_NodesModel), 0, LXType_Model, true, LXType_None, 0, 0, 0, }, 
{ "WaysModel", PIDX_WaysModel, offsetof(SectorGraphicalData, m_WaysModel), 0, LXType_Model, true, LXType_None, 0, 0, 0, }, 
{ "WaysModelsVector", PIDX_WaysModelsVector, offsetof(SectorGraphicalData, m_WaysModelsVector), 0, LXType_stdvector, false, LXType_Model, 0, 0, 0, }, 
{ "RoadEntities", PIDX_RoadEntities, offsetof(SectorGraphicalData, m_RoadEntities), 0, LXType_stdvector, false, LXType_Entity, 0, 0, 0, }, 
{ "StaticEntities", PIDX_StaticEntities, offsetof(SectorGraphicalData, m_StaticEntities), 0, LXType_stdvector, false, LXType_Entity, 0, 0, 0, }, 
{ "SplatMapTexture", PIDX_SplatMapTexture, offsetof(SectorGraphicalData, m_SplatMapTexture), 0, LXType_Texture, true, LXType_None, 0, 0, 0, }, 
{ "AlbedoTexture", PIDX_AlbedoTexture, offsetof(SectorGraphicalData, m_AlbedoTexture), 0, LXType_Texture, true, LXType_None, 0, 0, 0, }, 
};
bool SectorGraphicalData::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE SectorGraphicalData

SectorGraphicalData::SectorGraphicalData()
{
	m_AlbedoTexture = new Texture(10691);
	m_SplatMapTexture = new Texture();

	unsigned char* data = m_SplatMapTexture->GetTextureData();
	int numBytes = 4 * m_SplatMapTexture->GetSize().x * m_SplatMapTexture->GetSize().y;
	memset(data, 0, numBytes);
	m_SplatMapTexture->UpdateFromData();

}
