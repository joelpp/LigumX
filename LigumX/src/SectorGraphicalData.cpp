#include "SectorGraphicalData.h"
#include "Mesh.h"
#include "Texture.h"
#include "EngineSettings.h"

#pragma region  CLASS_SOURCE SectorGraphicalData

#include "SectorGraphicalData.h"
#include "serializer.h"
#include "Model.h"
#include "Entity.h"
#include "Texture.h"
const ClassPropertyData SectorGraphicalData::g_Properties[] = 
{
{ "NodesModel", PIDX_NodesModel, offsetof(SectorGraphicalData, m_NodesModel), 0, LXType_ObjectPtr, sizeof(Model*), LXType_Model, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "WaysModel", PIDX_WaysModel, offsetof(SectorGraphicalData, m_WaysModel), 0, LXType_ObjectPtr, sizeof(Model*), LXType_Model, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "WaysModelsVector", PIDX_WaysModelsVector, offsetof(SectorGraphicalData, m_WaysModelsVector), 0, LXType_stdvector, sizeof(std::vector<Model*>), LXType_stdvector, false, LXType_Model, true, 0, 0, 0, 0,}, 
{ "RoadEntities", PIDX_RoadEntities, offsetof(SectorGraphicalData, m_RoadEntities), 0, LXType_stdvector, sizeof(std::vector<Entity*>), LXType_stdvector, false, LXType_Entity, true, 0, 0, 0, 0,}, 
{ "StaticEntities", PIDX_StaticEntities, offsetof(SectorGraphicalData, m_StaticEntities), 0, LXType_stdvector, sizeof(std::vector<Entity*>), LXType_stdvector, false, LXType_Entity, true, 0, 0, 0, 0,}, 
{ "SplatMapTexture", PIDX_SplatMapTexture, offsetof(SectorGraphicalData, m_SplatMapTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "AlbedoTexture", PIDX_AlbedoTexture, offsetof(SectorGraphicalData, m_AlbedoTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
};
void SectorGraphicalData::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeObjectPtr("NodesModel", m_NodesModel);
	serializer.SerializeObjectPtr("WaysModel", m_WaysModel);
	serializer.SerializeVector("WaysModelsVector", m_WaysModelsVector);
	serializer.SerializeVector("RoadEntities", m_RoadEntities);
	serializer.SerializeVector("StaticEntities", m_StaticEntities);
	serializer.SerializeObjectPtr("SplatMapTexture", m_SplatMapTexture);
	serializer.SerializeObjectPtr("AlbedoTexture", m_AlbedoTexture);
}
bool SectorGraphicalData::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool SectorGraphicalData::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObjectPtr("NodesModel", m_NodesModel  );
	ImguiHelpers::ShowObjectPtr("WaysModel", m_WaysModel  );
	ImguiHelpers::ShowVector("WaysModelsVector", m_WaysModelsVector  );
	ImguiHelpers::ShowVector("RoadEntities", m_RoadEntities  );
	ImguiHelpers::ShowVector("StaticEntities", m_StaticEntities  );
	ImguiHelpers::ShowObjectPtr("SplatMapTexture", m_SplatMapTexture  );
	ImguiHelpers::ShowObjectPtr("AlbedoTexture", m_AlbedoTexture  );
	return true;
}
const char* SectorGraphicalData::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE SectorGraphicalData

SectorGraphicalData::SectorGraphicalData()
{
	m_AlbedoTexture = new Texture(10691);

	m_SplatMapTexture = g_ObjectManager->CreateObject<Texture>();
	m_SplatMapTexture->SetSize(glm::ivec2(512, 512));
	m_SplatMapTexture->Initialize();

	unsigned char* data = m_SplatMapTexture->GetTextureData();
	int numBytes = 4 * m_SplatMapTexture->GetSize().x * m_SplatMapTexture->GetSize().y;
	memset(data, 0, numBytes);
	m_SplatMapTexture->UpdateFromData();

}
