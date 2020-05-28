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

Model*& GetModel() { return m_Model; }; 
void SetModel(Model* value) { m_Model = value; }; 
private:
Model* m_Model = nullptr;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_VisualPIDX
{
PIDX_Model,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER Visual



};
