#include "ImguiManager.h"
#include "ImguiPointerDisplay.h"

#pragma region  CLASS_SOURCE ImguiManager

#include "ImguiManager.h"
#include "serializer.h"
void ImguiManager::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
}
bool ImguiManager::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool ImguiManager::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	return true;
}
const char* ImguiManager::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE ImguiManager


void ImguiManager::BeginFrame()
{
	m_LastImguiPointerDisplays.clear();

	for (ImguiPointerDisplay* oldPtrDisplay : m_ImguiPointerDisplays)
	{
		if (oldPtrDisplay->GetWasUsedThisFrame())
		{
			m_LastImguiPointerDisplays.push_back(oldPtrDisplay);
		}
		else
		{
			delete oldPtrDisplay;
		}
	}

	m_ImguiPointerDisplays.clear();

}

ImguiPointerDisplay& ImguiManager::GetPointerDisplay(void* baseObj, const ClassPropertyData& propertyData, ObjectID currentlySetObjectID)
{
	LXObject* obj = (LXObject*)baseObj + propertyData.m_Offset;
	for (ImguiPointerDisplay* ptrDisplay : m_LastImguiPointerDisplays)
	{
		if (ptrDisplay->GetDataPointer() == obj)
		{
			return *ptrDisplay;
		}
	}

	ImguiPointerDisplay* newPtrDisplay = new ImguiPointerDisplay();
	newPtrDisplay->SetCurrentID(currentlySetObjectID);
	newPtrDisplay->SetDataPointer(obj);
	newPtrDisplay->SetWasUsedThisFrame(true);
	m_ImguiPointerDisplays.push_back(newPtrDisplay); // todo jpp do perf vector resize stuff
	return *newPtrDisplay;
}


