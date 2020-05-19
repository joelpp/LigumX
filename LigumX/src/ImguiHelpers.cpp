#include "ImguiHelpers.h"
#include "Imguimanager.h"
#include "ImguiPointerDisplay.h"
#include <glm/gtx/string_cast.hpp>



//static int g_NumObjectPtrsShown[32];
//static int g_ObjectPtrID = 0;
//
//void ImguiHelpers::BeginFrame()
//{
//	for (int i = 0; i < 32; ++i)
//	{
//		g_NumObjectPtrsShown[i] = -1;
//	}
//
//	g_ObjectPtrID = 0;
//}



bool ImguiHelpers::ShowBool(const char* name, bool& value)
{
	return ImGui::Checkbox(name, &value);
}

bool ImguiHelpers::ShowBool(LXString& name, bool& value)
{
	return ImguiHelpers::ShowBool(name.c_str(), value);
}


bool ImguiHelpers::ShowInt(const char* name, int& value, int min, int max)
{
	return ImGui::DragInt(name, &value, 1.f, min, max);
}
bool ImguiHelpers::ShowInt(LXString& name, int& value, int min, int max)
{
	return ShowInt(name.c_str(), value, min, max);
}

bool ImguiHelpers::ShowInt64(const char* name, lxInt64& value, long min, long max)
{
	// todo fix this...
	std::string val = std::to_string(value);
	return ImguiHelpers::ShowString(name, val);
	//return ImGui::DragInt(name, (int*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowInt64(LXString& name, lxInt64& value, long min, long max)
{
	return ShowInt64(name.c_str(), value, min, max);
}

bool ImguiHelpers::ShowIVec2(const char* name, glm::ivec2& value, int min, int max)
{
	return ImGui::DragInt2(name, (int*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowIVec2(LXString& name, glm::ivec2& value, int min, int max)
{
	return ShowIVec2(name.c_str(), value, min, max);
}

bool ImguiHelpers::ShowHighp_IVec2(const char* name, glm::highp_ivec2& value, int min, int max)
{
	std::string val = std::to_string(value.x) + " " + std::to_string(value.y);
	return ImguiHelpers::ShowString(name, val);
}

bool ImguiHelpers::ShowHighp_IVec2(LXString& name, glm::highp_ivec2& value, int min, int max)
{
	return ShowHighp_IVec2(name.c_str(), value, min, max);
}

bool ImguiHelpers::ShowFloat(const char* name, float& value, float min, float max)
{
	return ImGui::DragFloat(name, &value, 1.f, min, max);
}

bool ImguiHelpers::ShowFloat(LXString& name, float& value, float min, float max)
{
	return ImguiHelpers::ShowFloat(name.c_str(), value, min, max);
}

bool ImguiHelpers::ShowVec2(const char* name, glm::vec2& value, float min, float max)
{
	return ImGui::DragFloat2(name, (float*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowVec2(LXString& name, glm::vec2& value, float min, float max)
{
	return ImguiHelpers::ShowVec2(name.c_str(), value, min, max);
}


bool ImguiHelpers::ShowVec3(const char* name, glm::vec3& value, float min, float max)
{
	return ImGui::DragFloat3(name, (float*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowVec3(LXString& name, glm::vec3& value, float min, float max)
{
	return ImguiHelpers::ShowVec3(name.c_str(), value, min, max);
}


bool ImguiHelpers::ShowVec4(const char* name, glm::vec4& value, float min, float max)
{
	return ImGui::DragFloat4(name, (float*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowVec4(LXString& name, glm::vec4& value, float min, float max)
{
	return ImguiHelpers::ShowVec4(name.c_str(), value, min, max);
}


bool ImguiHelpers::ShowString(const char* name, LXString& value)
{
	ImGui::Text("%s : %s", name, value.c_str());
	return true;
}

bool ImguiHelpers::ShowString(LXString& name, LXString& value)
{
	return ImguiHelpers::ShowString(name.c_str(), value);
}

bool ImguiHelpers::ShowRawString(const char* str)
{
	ImGui::Text("%s", str);
	return true;
}

bool ImguiHelpers::ShowRawString(LXString& str)
{
	return ShowRawString(str.c_str());
}

bool ImguiHelpers::ShowVector(const char* name, std::vector<std::string>& values)
{
	return false;
}

bool ImguiHelpers::BeginPropertyTree(const char* name)
{
	// one could have see having an enum to control how the property grid will be rendered
	// i.e. menu, own window with treenode, treenode only, __just the properties__
	return ImGui::TreeNode(name);
}

void ImguiHelpers::EndPropertyTree()
{
	ImGui::TreePop();
}

bool ImguiHelpers::ShowObjectPtr(const char* name, glm::vec3* value)
{
	CommonPrimitiveShowPtr(name, "glm::vec3", value);
	return true;
}
bool ImguiHelpers::ShowObjectPtr(const char* name, glm::vec2* value)
{
	CommonPrimitiveShowPtr(name, "glm::vec2", value);
	return true;
}
bool ImguiHelpers::ShowObjectPtr(const char* name, int* value)
{
	CommonPrimitiveShowPtr(name, "int", value);
	return true;
}



////////



void ImguiHelpers::ShowVariableAsText(glm::vec3 variable, const char* variableName)
{
	ImGui::Text("%s : %f, %f, %f", variableName, variable.x, variable.y, variable.z);
}

void ImguiHelpers::ShowVariableAsText(glm::vec3* variable, const char* variableName)
{
	ImGui::Text("%s : %f, %f, %f", variableName, variable->x, variable->y, variable->z);
}

void ImguiHelpers::ShowGUIText(const char* text)
{
	ImGui::Text("%s", text);
}

void ImguiHelpers::ShowGUIText(const std::string& text)
{
	ImGui::Text("%s", text.c_str());
}

void ImguiHelpers::ShowGUIText(std::string* text)
{
	ImGui::Text("%s", text->c_str());
}

void ImguiHelpers::ShowGUIText(std::string* text, const char* variableName)
{
	ImGui::Text("%s : %s", variableName, text->c_str());
}

void ImguiHelpers::ShowGUIText(const std::string& text, const char* variableName)
{
	ImGui::Text("%s : %s", variableName, text.c_str());
}

void ImguiHelpers::ShowVariableAsText(int variable, const char* variableName)
{
	ImGui::Text("%s : %d", variableName, variable);
}

void ImguiHelpers::ShowVariableAsText(float variable, const char* variableName)
{
	ImGui::Text("%s : %f", variableName, variable);
}



bool ImguiHelpers::ShowProperty(int* value, const char* name)
{
	ShowVariableAsText(*value, name);

	return false;
}

bool ImguiHelpers::ShowProperty(int* value, const char* name, int min, int max)
{
	return ImGui::SliderInt(name, value, min, max);
}

bool ImguiHelpers::ShowProperty(bool* value, const char* name)
{
	return ImGui::Checkbox(name, value);
}

bool ImguiHelpers::ShowProperty(bool& value, const char* name)
{
	return ImGui::Checkbox(name, &value);
}
//
//void Editor::ShowProperty(std::vector<bool>::reference value, const char* name)
//{
//	ImGui::Checkbox(name, valuer);
//}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, bool* value)
{
	bool changed = ShowBool(propertyData.m_Name, *value);
	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, int* value, float min, float max)
{
	bool changed = ShowInt(propertyData.m_Name, *value, (int)min, (int)max);

	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, lxInt64* value, float min, float max)
{
	bool changed = ShowInt64(propertyData.m_Name, *value, (long)min, (long)max);

	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, float* value, float min, float max)
{
	bool changed = false;
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		changed = ImGui::DragFloat(propertyData.m_Name, (float*)value);
	}
	else
	{
		changed = ImGui::SliderFloat(propertyData.m_Name, value, min, max);
	}
	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;

}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, glm::vec2* value, float min, float max)
{
	bool changed = false;
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		changed = ImGui::DragFloat2(propertyData.m_Name, (float*)value);
	}
	else
	{
		changed = ImGui::SliderFloat2(propertyData.m_Name, (float*)value, min, max);
	}
	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, glm::highp_ivec2* value, float min, float max)
{
	return false;
}


bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, glm::vec3* value, float min, float max)
{
	bool changed = false;
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		changed = ImGui::DragFloat3(propertyData.m_Name, (float*)value);
	}
	else
	{
		changed = ImGui::SliderFloat3(propertyData.m_Name, (float*)value, min, max);
	}
	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}

	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, glm::vec4* value, float min, float max)
{
	bool changed = false;
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		changed = ImGui::DragFloat4(propertyData.m_Name, (float*)value);
	}
	else
	{
		changed = ImGui::SliderFloat4(propertyData.m_Name, (float*)value, min, max);
	}
	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, glm::ivec2* value, float min, float max)
{
	bool changed = false;
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		changed = ImGui::DragInt2(propertyData.m_Name, (int*)value);
	}
	else
	{
		changed = ImGui::SliderInt2(propertyData.m_Name, (int*)value, (int)min, (int)max);
	}

	if (changed && (propertyData.m_PropertyFlags & PropertyFlags_SetCallback))
	{
		propertyData.m_WriteCallback((char*)object, (char*)value);
	}
	return changed;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, std::string* value)
{
	return ShowString(propertyData.m_Name, *value);
}


//bool ImguiHelpers::ShowProperty(std::string* value, const char* name)
//{
//	//ShowGUIText(value, name);
//	//return false;
//	return false;
//}

//bool ImguiHelpers::ShowEditableProperty(int* ptr, const char* name)
//{
//	int val = *((int*)ptr);
//	std::string sCopy = std::to_string(val);
//
//	static char buf_int[64] = "";
//	memset(buf_int, 0, 64);
//	memcpy(buf_int, sCopy.c_str(), sCopy.size() * sizeof(char));
//
//	bool changed = ImGui::InputText(name, buf_int, 64, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal);
//	if (changed)
//	{
//		*((int*)ptr) = std::atoi(buf_int);
//	}
//
//	return changed;
//}

template <typename T, typename U>
bool ImguiHelpers::ShowProperty(std::map<U, char*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	return false;
}

template <typename T, typename U>
bool ImguiHelpers::ShowProperty(std::map<U, T*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	return false;

}

template <typename T, typename U>
bool ImguiHelpers::ShowProperty(std::unordered_map<U, char*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	return false;

}

template <typename T, typename U>
bool ImguiHelpers::ShowProperty(std::unordered_map<U, T*>* map, const char* name)
{
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		for (auto it = map->begin(); it != map->end(); ++it)
		{
			T* obj = (T*)it->second;
			std::string label = obj->GetName() + " [" + std::to_string(it->first) + "]";
			ShowPropertyGridTemplate(obj, label.c_str());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	return false;
}

bool ImguiHelpers::ShowObject(void* object, const ClassPropertyData& propertyData, LXObject*& value)
{
	ImguiManager& imguiManager = ImguiManager::GetInstance();

	ImguiPointerDisplay& ptrDisplay = imguiManager.GetPointerDisplay(object, propertyData, value->GetObjectID());

	ShowObjectPtr(propertyData.m_Name, value);

	bool changeObjectRequested = false;
	bool hasOpenPopup = ptrDisplay.GetOpenPopup();
	if (ImGui::BeginPopupContextItem("Object"))
	{
		if (ImGui::Selectable("Browse..."))
		{
			ptrDisplay.SetOpenPopup(true);

		}
		ImGui::EndPopup();
	}

	if (!hasOpenPopup && ptrDisplay.GetOpenPopup())
	{
		ImGui::OpenPopup("Select file to use");
		ptrDisplay.SetOpenPopup(false);
	}

	if (ImGui::BeginPopupModal("Select file to use", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Select a file from the list:");

		// todo jpp : this could be cleaned up into a "select lx file modal popup" all fancy
		{
			const std::vector<FileDisplayInformation>& allFiles = g_ObjectManager->GetAllFiles();
			std::vector<FileDisplayInformation> filteredFiles;
			// keep only files with compatible type
			// todo jpp : fix this for inheritance, perf
			for (const FileDisplayInformation& file : allFiles)
			{
				if (file.m_Typename == value->GetLXClassName())
				{
					filteredFiles.push_back(file);
				}
			}

			int numItems = filteredFiles.size();
			int selectedFileIndex = imguiManager.GetSelectedFileIndex();
			ImGui::ListBox("Symbols", &selectedFileIndex, VectorOfFileDisplayInfoGetter, (void*)&filteredFiles, numItems);
			imguiManager.SetSelectedFileIndex(selectedFileIndex);

			if (ImGui::Button("Ok"))
			{
				LXObject* newObject = g_ObjectManager->GetObjectFromIDAndType(true, filteredFiles[selectedFileIndex].m_ObjectID, filteredFiles[selectedFileIndex].m_Typename);
				value = newObject;

				imguiManager.SetSelectedFileIndex(-1);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				imguiManager.SetSelectedFileIndex(-1);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (ImGui::InputInt("ID", &(ptrDisplay.GetObjectIDRef()), 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LXObject* newObject = g_ObjectManager->GetObjectFromIDAndType(true, ptrDisplay.GetCurrentID(), value->GetLXClassName());
		value = newObject;
	}

	return true;
}

bool ImguiHelpers::ShowObjectPtr(const char* name, LXObject*& value)
{
		if (value == nullptr)
		{
			std::string treeNodeName = lxFormat("%s : unset (nullptr)", name);
			ShowRawString(treeNodeName);
		}
		else
		{
			std::string treeNodeName = lxFormat("%s : %s [%s] (%d)", name, value->GetName().c_str(), value->GetLXClassName(), value->GetObjectID());

			ImguiTreeNodeScope scope(treeNodeName.c_str());

			bool success = scope.m_Opened;

			if (success)
			{
				success = value->ShowPropertyGrid();
			}

			return success;
		}

		return true;
}

bool ImguiHelpers::ShowProperty(void* object, const ClassPropertyData& propertyData, LXObject* value)
{
	return ShowObjectPtr(propertyData.m_Name, value);
}

bool ImguiHelpers::VectorOfFileDisplayInfoGetter(void* data, int n, const char** out_text)
{
	std::vector<FileDisplayInformation>* v = (std::vector<FileDisplayInformation>*)data;

	FileDisplayInformation& fileInfo = (*v)[n];
	*out_text = fileInfo.m_AsText.c_str();

	return true;
}
