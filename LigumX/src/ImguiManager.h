#pragma once

#pragma region  FORWARD_DECLARATIONS ImguiManager
#include "LXSystem.h"
class Serializer2;

class ImguiManager;


#pragma endregion  FORWARD_DECLARATIONS ImguiManager

class ImguiPointerDisplay;

#pragma region  HEADER ImguiManager
class ImguiManager : public LXObject
{
public:
static const int ClassID = -1368442899;
static const LXType Type = LXType_ImguiManager;
static constexpr const char* ClassName = "ImguiManager";
virtual LXType GetLXType() { return LXType_ImguiManager; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

int GetSelectedFileIndex() { return m_SelectedFileIndex; }; 
void SetSelectedFileIndex(int value) { m_SelectedFileIndex = value; }; 
private:
int m_SelectedFileIndex = -1;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ImguiManagerPIDX
{
PIDX_SelectedFileIndex,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(ImguiManager& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER ImguiManager

public:
	static ImguiManager& GetInstance()
	{
		static ImguiManager instance; // Guaranteed to be destroyed.
								// Instantiated on first use.
		return instance;
	}

	void BeginFrame();
	std::vector<ImguiPointerDisplay*> m_ImguiPointerDisplays;
	std::vector<ImguiPointerDisplay*> m_LastImguiPointerDisplays;
	ImguiPointerDisplay& GetPointerDisplay(LXObject* baseObj, const ClassPropertyData& propertyData, ObjectID currentlySetObjectID);

};
