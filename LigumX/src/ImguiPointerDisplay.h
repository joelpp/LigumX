#pragma once

#pragma region  FORWARD_DECLARATIONS ImguiPointerDisplay
#include "LXSystem.h"
class Serializer2;

class ImguiPointerDisplay;
class LXObject;


#pragma endregion  FORWARD_DECLARATIONS ImguiPointerDisplay
#pragma region  HEADER ImguiPointerDisplay
class ImguiPointerDisplay : public LXObject
{
public:
static const int ClassID = -1203610705;
static const LXType Type = LXType_ImguiPointerDisplay;
static constexpr const char* ClassName = "ImguiPointerDisplay";
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

int GetCurrentID() { return m_CurrentID; }; 
void SetCurrentID(int value) { m_CurrentID = value; }; 
LXObject*& GetDataPointer() { return m_DataPointer; }; 
void SetDataPointer(LXObject* value) { m_DataPointer = value; }; 
bool GetWasUsedThisFrame() { return m_WasUsedThisFrame; }; 
void SetWasUsedThisFrame(bool value) { m_WasUsedThisFrame = value; }; 
bool GetOpenPopup() { return m_OpenPopup; }; 
void SetOpenPopup(bool value) { m_OpenPopup = value; }; 
private:
int m_CurrentID = 0;
LXObject* m_DataPointer = nullptr;
bool m_WasUsedThisFrame = false;
bool m_OpenPopup = false;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ImguiPointerDisplayPIDX
{
PIDX_CurrentID,
PIDX_DataPointer,
PIDX_WasUsedThisFrame,
PIDX_OpenPopup,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER ImguiPointerDisplay

public:

ObjectID& GetObjectIDRef() { return m_CurrentID; }


};
