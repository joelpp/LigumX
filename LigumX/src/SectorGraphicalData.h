#pragma once
#include <vector>

#pragma region  FORWARD_DECLARATIONS SectorGraphicalData
#include "LXSystem.h"
class Serializer2;

class SectorGraphicalData;
class Model;
class Entity;
class Texture;


#pragma endregion  FORWARD_DECLARATIONS SectorGraphicalData

#pragma region  HEADER SectorGraphicalData
class SectorGraphicalData : public LXObject
{
public:
static const int ClassID = 485409280;
static const LXType Type = LXType_SectorGraphicalData;
static constexpr const char* ClassName = "SectorGraphicalData";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

Model*& GetNodesModel() { return m_NodesModel; }; 
void SetNodesModel(Model* value) { m_NodesModel = value; }; 
Model*& GetWaysModel() { return m_WaysModel; }; 
void SetWaysModel(Model* value) { m_WaysModel = value; }; 
std::vector<Model*>& GetWaysModelsVector() { return m_WaysModelsVector; }; 
void SetWaysModelsVector(std::vector<Model*> value) { m_WaysModelsVector = value; }; 
void AddTo_WaysModelsVector(Model* value) { m_WaysModelsVector.push_back(value); };
std::vector<Entity*>& GetRoadEntities() { return m_RoadEntities; }; 
void SetRoadEntities(std::vector<Entity*> value) { m_RoadEntities = value; }; 
void AddTo_RoadEntities(Entity* value) { m_RoadEntities.push_back(value); };
std::vector<Entity*>& GetStaticEntities() { return m_StaticEntities; }; 
void SetStaticEntities(std::vector<Entity*> value) { m_StaticEntities = value; }; 
void AddTo_StaticEntities(Entity* value) { m_StaticEntities.push_back(value); };
Texture*& GetSplatMapTexture() { return m_SplatMapTexture; }; 
void SetSplatMapTexture(Texture* value) { m_SplatMapTexture = value; }; 
Texture*& GetAlbedoTexture() { return m_AlbedoTexture; }; 
void SetAlbedoTexture(Texture* value) { m_AlbedoTexture = value; }; 
private:
Model* m_NodesModel = nullptr;
Model* m_WaysModel = nullptr;
std::vector<Model*> m_WaysModelsVector;
std::vector<Entity*> m_RoadEntities;
std::vector<Entity*> m_StaticEntities;
Texture* m_SplatMapTexture = nullptr;
Texture* m_AlbedoTexture = nullptr;
public:
static const int g_PropertyCount = 7;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorGraphicalDataPIDX
{
PIDX_NodesModel,
PIDX_WaysModel,
PIDX_WaysModelsVector,
PIDX_RoadEntities,
PIDX_StaticEntities,
PIDX_SplatMapTexture,
PIDX_AlbedoTexture,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER SectorGraphicalData

SectorGraphicalData();

};
