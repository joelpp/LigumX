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
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

private:
public:
static const int g_PropertyCount = 0;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_ImguiManagerPIDX
{
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
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
	ImguiPointerDisplay& GetPointerDisplay(void* baseObj, const ClassPropertyData& propertyData, ObjectID currentlySetObjectID);

};
