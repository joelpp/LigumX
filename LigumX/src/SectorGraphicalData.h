#pragma once
#include <vector>

#pragma region  FORWARD_DECLARATIONS SectorGraphicalData
#include "property.h"

class SectorGraphicalData;
class Model;
class Entity;
class Texture;


#pragma endregion  FORWARD_DECLARATIONS SectorGraphicalData

class SectorGraphicalData
{
#pragma region  HEADER SectorGraphicalData
public:
static const int ClassID = 1523090688;
static const LXType Type = LXType_SectorGraphicalData;
static constexpr const char* ClassName = "SectorGraphicalData";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
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
Texture*& GetSplatMapTexture() { return m_SplatMapTexture; }; 
void SetSplatMapTexture(Texture* value) { m_SplatMapTexture = value; }; 
Texture*& GetAlbedoTexture() { return m_AlbedoTexture; }; 
void SetAlbedoTexture(Texture* value) { m_AlbedoTexture = value; }; 
private:
int m_ObjectID;
std::string m_Name;
Model* m_NodesModel;
Model* m_WaysModel;
std::vector<Model*> m_WaysModelsVector;
std::vector<Entity*> m_RoadEntities;
Texture* m_SplatMapTexture;
Texture* m_AlbedoTexture;
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_SectorGraphicalDataPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_NodesModel,
PIDX_WaysModel,
PIDX_WaysModelsVector,
PIDX_RoadEntities,
PIDX_SplatMapTexture,
PIDX_AlbedoTexture,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SectorGraphicalData

SectorGraphicalData();

};
