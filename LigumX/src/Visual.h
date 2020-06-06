#pragma once

#include "Component.h"

#pragma region  FORWARD_DECLARATIONS Visual
#include "LXSystem.h"
class Serializer2;

class Visual;
class Model;


#pragma endregion  FORWARD_DECLARATIONS Visual
#pragma region  HEADER Visual
class Visual : public Component
{
public:
static const int ClassID = -2060904583;
static const LXType Type = LXType_Visual;
static constexpr const char* ClassName = "Visual";
virtual LXType GetLXType() { return LXType_Visual; }
virtual const char* GetLXClassName() { return ClassName; }
typedef Component super;

std::vector<Model*>& GetModels() { return m_Models; }; 
void SetModels(std::vector<Model*> value) { m_Models = value; }; 
void AddTo_Models(Model* value) { m_Models.push_back(value); };
private:
std::vector<Model*> m_Models;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_VisualPIDX
{
PIDX_Models,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER Visual



};
