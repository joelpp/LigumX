#pragma once

#pragma region  FORWARD_DECLARATIONS MaterialTool
#include "LXSystem.h"
class Serializer2;

class MaterialTool;
class Entity;
class Material;


#pragma endregion  FORWARD_DECLARATIONS MaterialTool

#include "EditorTool.h"

#pragma region  HEADER MaterialTool
class MaterialTool : public EditorTool
{
public:
static const int ClassID = -962473574;
static const LXType Type = LXType_MaterialTool;
static constexpr const char* ClassName = "MaterialTool";
virtual LXType GetLXType() { return LXType_MaterialTool; }
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

Entity*& GetPickedEntity() { return m_PickedEntity; }; 
void SetPickedEntity(Entity* value) { m_PickedEntity = value; }; 
std::vector<Material*>& GetPickedMaterials() { return m_PickedMaterials; }; 
void SetPickedMaterials(std::vector<Material*> value) { m_PickedMaterials = value; }; 
void AddTo_PickedMaterials(Material* value) { m_PickedMaterials.push_back(value); };
private:
Entity* m_PickedEntity = nullptr;
std::vector<Material*> m_PickedMaterials;
public:
static const int g_PropertyCount = 2;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_MaterialToolPIDX
{
PIDX_PickedEntity,
PIDX_PickedMaterials,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER MaterialTool

virtual bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);

virtual void DrawImguiWindow();

};
