#include "ImguiManager.h"
#include "ImguiPointerDisplay.h"

#pragma region  CLASS_SOURCE ImguiManager

#include "ImguiManager.h"
#include "serializer.h"
const ClassPropertyData ImguiManager::g_Properties[] = 
{
{ "SelectedFileIndex", PIDX_SelectedFileIndex, offsetof(ImguiManager, m_SelectedFileIndex), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
};
void ImguiManager::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_SelectedFileIndex], m_SelectedFileIndex);
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
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_SelectedFileIndex], &m_SelectedFileIndex , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	return true;
}
void ImguiManager::Clone(ImguiManager& other)
{
	super::Clone(other);
	other.SetSelectedFileIndex(m_SelectedFileIndex);
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

ImguiPointerDisplay& ImguiManager::GetPointerDisplay(LXObject* baseObj, const ClassPropertyData& propertyData, ObjectID currentlySetObjectID)
{
	for (ImguiPointerDisplay* ptrDisplay : m_LastImguiPointerDisplays)
	{
		if (ptrDisplay->GetDataPointer() == baseObj)
		{
			return *ptrDisplay;
		}
	}

	ImguiPointerDisplay* newPtrDisplay = new ImguiPointerDisplay();
	newPtrDisplay->SetCurrentID(currentlySetObjectID);
	newPtrDisplay->SetDataPointer(baseObj);
	newPtrDisplay->SetWasUsedThisFrame(true);
	m_ImguiPointerDisplays.push_back(newPtrDisplay); // todo jpp do perf vector resize stuff
	return *newPtrDisplay;
}


