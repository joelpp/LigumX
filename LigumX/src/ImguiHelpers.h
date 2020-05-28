#pragma once

#include "LXSystem.h"
#include "imgui.h"

class LXObject;

struct ClassPropertyData;
// todo : overload macros based on number of arguments? 

#define LXIMGUI_SHOW_BOOL(name, value) ImguiHelpers::ShowBool(name, value)
#define LXIMGUI_SHOW_BOOL_SETCALLBACK(name, value) ImguiHelpers::ShowBool(name, value)

#define LXIMGUI_SHOW_INT(name, value, min, max) ImguiHelpers::ShowInt(name, value, min, max)
#define LXIMGUI_SHOW_LONG(name, value, min, max) ImguiHelpers::ShowLong(name, value, min, max)
#define LXIMGUI_SHOW_IVEC2(name, value, min, max) ImguiHelpers::ShowIVec2(name, value, min, max)


#define LXIMGUI_SHOW_FLOAT(name, value, min, max) ImguiHelpers::ShowFloat(name, value, min, max)
#define LXIMGUI_SHOW_FLOAT_SETCALLBACK(name, value, min, max) ImguiHelpers::ShowFloat(name, value, min, max)
#define LXIMGUI_SHOW_VEC2(name, value, min, max) ImguiHelpers::ShowVec2(name, value, min, max)
#define LXIMGUI_SHOW_VEC2_SETCALLBACK(name, value, min, max) ImguiHelpers::ShowVec2(name, value, min, max)
#define LXIMGUI_SHOW_VEC3(name, value, min, max) ImguiHelpers::ShowVec3(name, value, min, max)
#define LXIMGUI_SHOW_VEC3_SETCALLBACK(name, value, min, max) ImguiHelpers::ShowVec3(name, value, min, max)
#define LXIMGUI_SHOW_VEC4(name, value, min, max) ImguiHelpers::ShowVec4(name, value, min, max)
#define LXIMGUI_SHOW_VEC4_SETCALLBACK(name, value, min, max) ImguiHelpers::ShowVec4(name, value, min, max)
#define LXIMGUI_SHOW_STRING(name, value) ImguiHelpers::ShowString(name, value)

#define LXIMGUI_SHOW_OBJECTPTR_VECTOR(name, value) ImguiHelpers::ShowVector(name, value)
#define LXIMGUI_SHOW_OBJECT_VECTOR(name, value) ImguiHelpers::ShowVector(name, value)

#define LXIMGUI_SHOW_OBJECTREF(name, value) ImguiHelpers::ShowObjectPtr(name, value);
#define LXIMGUI_SHOW_OBJECTREF_SETCALLBACK(name, value) ImguiHelpers::ShowObjectPtr(name, value);

class ImguiIDScope
{
public: 

	ImguiIDScope(const char* name)
	{
		ImGui::PushID(name);
	}

	~ImguiIDScope()
	{
		ImGui::PopID();
	}


};

class ImguiTreeNodeScope
{
public:
	ImguiTreeNodeScope(const char* name)
		: m_IDScope(name)
		, m_Opened(ImGui::TreeNode(name))
	{
		
	}

	~ImguiTreeNodeScope()
	{
		if (m_Opened)
		{
			ImGui::TreePop();
		}
	}

	bool m_Opened;
	ImguiIDScope m_IDScope;
};

class ImguiMenuScope
{
public:
	ImguiMenuScope(const char* name)
		: m_IDScope(name)
	{
		m_Opened = ImGui::BeginMenu(name);
	}

	~ImguiMenuScope()
	{
		if (m_Opened)
		{
			ImGui::EndMenu();
		}
	}

	bool m_Opened;
	ImguiIDScope m_IDScope;
};

namespace ImguiHelpers
{
	void BeginFrame();

	bool ShowBool(const char* name, bool& value);
	bool ShowBool(LXString& name, bool& value);

	bool ShowInt(const char* name, int& value, int min, int max);
	bool ShowInt(LXString& name, int& value, int min, int max);

	bool ShowInt64(const char* name, lxInt64& value, long min, long max);
	bool ShowInt64(LXString& name, lxInt64& value, long min, long max);

	bool ShowIVec2(const char* name, glm::ivec2& value, int min, int max);
	bool ShowIVec2(LXString& name, glm::ivec2& value, int min, int max);

	bool ShowHighp_IVec2(LXString& name, glm::highp_ivec2& value, int min, int max);
	bool ShowHighp_IVec2(const char* name, glm::highp_ivec2& value, int min, int max);

	bool ShowFloat(const char* name, float& value, float min, float max);
	bool ShowFloat(LXString& name, float& value, float min, float max);

	bool ShowVec2(const char* name, glm::vec2& value, float min, float max);
	bool ShowVec2(LXString& name, glm::vec2& value, float min, float max);

	bool ShowVec3(const char* name, glm::vec3& value, float min, float max);
	bool ShowVec3(LXString& name, glm::vec3& value, float min, float max);

	bool ShowVec4(const char* name, glm::vec4& value, float min, float max);
	bool ShowVec4(LXString& name, glm::vec4& value, float min, float max);

	//template <typename T>
	//ShowProperty(void* srcObject, ClassPropertyData& propertyData, T& value, float min, float max)
	//{
		//bool changed = 
	//}

	bool ShowString(const char* name, LXString& value);
	bool ShowString(LXString& name, LXString& value);

	bool ShowRawString(const char* str);
	bool ShowRawString(LXString& str);

	bool BeginPropertyTree(const char* name);

	void EndPropertyTree();

	template <typename T>
	void CommonPrimitiveShowPtr(const char* name, const char* type, T* value)
	{
		bool isNull = (value == nullptr);
		char treeNodeName[256];
		sprintf(treeNodeName, "%s (%s)%s", name, type, (isNull ? " (nullptr)" : ""));

		if (isNull)
		{
			ShowRawString(treeNodeName);
		}
		else
		{
			ShowRawString("value_here");
		}

	}

	template <typename T>
	bool ShowObject2(void* object, const ClassPropertyData& propertyData, T** value)
	{
		LXObject** lxObjectPtrPtr = (LXObject**)value;
		LXObject*& lxObjectPtrRef = *lxObjectPtrPtr;
		return ShowObject(object, propertyData, lxObjectPtrRef);
	}

	bool ShowObject(void* object, const ClassPropertyData& propertyData, LXObject*& value);
	bool ShowObjectPtr(const char* name, glm::vec3* value);
	bool ShowObjectPtr(const char* name, glm::vec2* value);
	bool ShowObjectPtr(const char* name, int* value);

	bool ShowObjectPtr(const char* name, LXObject*& value);
	//template <typename T>
	//bool ShowObjectPtr(const char* name, T*& value)
	//{
	//	bool isNull = (value == nullptr);
	//	char treeNodeName[256];

	//	if (isNull)
	//	{
	//		sprintf(treeNodeName, "(nullptr)");
	//	}
	//	else
	//	{
	//		sprintf(treeNodeName, "%s [%s]", value->GetName().c_str(), value->GetLXClassName());
	//	}

	//	if (value == nullptr)
	//	{
	//		ShowRawString(treeNodeName);
	//	}
	//	else
	//	{
	//		// copy object id
	//		ObjectID ptrObjectID = value->GetObjectID();
	//		if (ImguiHelpers::ShowInt("ID", ptrObjectID, 0, LX_LIMITS_INT_MAX)) // display copy
	//		{
	//			// find or load object
	//			T* newObject = g_ObjectManager->GetObjectFromIDAndType(true, ptrObjectID, value->GetLXClassName());

	//			if (newObject)
	//			{
	//				value = newObject;
	//				return true;
	//			}
	//		}

	//		ImGui::SameLine();
	//		ImguiTreeNodeScope scope(treeNodeName);

	//		bool success = scope.m_Opened;

	//		if (success)
	//		{
	//			success = value->ShowPropertyGrid();
	//		}

	//		return success;
	//	}

	//	return true;
	//}

	template <typename T>
	bool ShowObjectPtr_SetCallback(const char* name, T* value)
	{
		return ShowObjectPtr<T>(name, value);
	}

	bool ShowVector(const char* name, std::vector<std::string>& values);

	template <typename T>
	bool ShowVector(const char* name, std::vector<T>& values)
	{
		size_t numElements = values.size();
		char treeNodeName[256];
		sprintf(treeNodeName, "%s (List<%s>, size = %zu)", name, "[Classname, Fix this!]"/*T::ClassName*/, numElements);

		if (numElements == 0)
		{
			ShowRawString(treeNodeName);
		}
		else
		{
			ImguiTreeNodeScope scope(treeNodeName);
			for (int i = 0; i < numElements; ++i)
			{
				T& objectRef = values[i];

				char thisObjectType[256];
				sprintf(thisObjectType, "%s", "someTypeNameToFix"/*objectRef.GetTypeName()*/);

				char elementName[256];
				//sprintf(elementName, "[%d] : %s (%s)", i, objectRef.GetName().c_str(), thisObjectType);
				sprintf(elementName, "[%d] : oh god fix this", i);

				ShowObjectPtr(elementName, &objectRef);
			}
		}

		return true;
	}

	template <typename T>
	bool ShowVector(const char* name, std::vector<T*>& values)
	{
		size_t numElements = values.size();
		char treeNodeName[256];
		sprintf(treeNodeName, "%s (List<%s>, size = %zu)", name, T::ClassName, numElements);

		if (numElements == 0)
		{
			ShowRawString(treeNodeName);
		}
		else
		{
			ImguiTreeNodeScope scope(treeNodeName);

			if (scope.m_Opened)
			{
				for (size_t i = 0; i < numElements; ++i)
				{
					T* objectPtr = values[i];

					char thisObjectType[256];
					sprintf(thisObjectType, "%s", objectPtr->GetTypeName());

					char elementName[256];
					sprintf(elementName, "[%zu] : (%s)", i, /*objectPtr->GetName(),*/ thisObjectType);

					ShowObjectPtr(elementName, objectPtr);
				}
			}
		}

		return true;
	}


	bool ShowProperty(int* value, const char* name);
	bool ShowProperty(bool* value, const char* name);
	bool ShowProperty(bool& value, const char* name);
	bool ShowProperty(int* value, const char* name, int min, int max);

	bool ShowProperty(std::vector<bool>::reference value, const char* name);

	bool ShowProperty(void* object, const ClassPropertyData& propertyData, bool* value);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, int* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, lxInt64* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, float* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, glm::vec4* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, glm::vec3* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, glm::vec2* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, glm::highp_ivec2* value, float min, float max);

	bool ShowProperty(void* object, const ClassPropertyData& propertyData, glm::ivec2* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, std::string* value);
	bool ShowString2(void* object, const ClassPropertyData& propertyData, std::string& value);

	bool ShowProperty(void* object, const ClassPropertyData& propertyData, LXObject* value);
	//template <typename T>
	//bool ShowProperty(void* object, const ClassPropertyData& propertyData, T* value)
	//{
	//	return ShowObjectPtr(propertyData.m_Name, value);
	//}
	template <typename T>
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, std::vector<T>& value)
	{
		return false;
	}

	bool ShowProperty2(void* object, const ClassPropertyData& propertyData, std::vector<LXObject*>& values);

	template <typename T>
	bool ShowProperty3(void* object, const ClassPropertyData& propertyData, std::vector<T*>& values)
	{
		std::vector<LXObject*>& v = *(std::vector<LXObject*> *)&values;
		return ShowProperty2(object, propertyData, v);
	}

	template <typename T>
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, std::vector<T*>& values)
	{
		std::string treeNodeName = lxFormat("%s (%s[%d])", propertyData.m_Name, T::ClassName, values.size());

		//ImguiManager& imguiManager = ImguiManager::GetInstance();

		static bool popupIsOpen = false;
		static int selectedFileIndex = -1;
		//ImguiPointerDisplay& ptrDisplay = imguiManager.GetPointerDisplay(object, propertyData, 0);

		ImguiTreeNodeScope scope(treeNodeName.c_str());

		bool changeObjectRequested = false;
		//bool hasOpenPopup = ptrDisplay.GetOpenPopup();
		bool hasOpenPopup = popupIsOpen;
		if (ImGui::BeginPopupContextItem("Object"))
		{
			if (ImGui::Selectable("Add..."))
			{
				//ptrDisplay.SetOpenPopup(true);
				popupIsOpen = true;
			}
			ImGui::EndPopup();
		}

		bool success = scope.m_Opened;

		if (success)
		{
			for (int i = 0; i < values.size(); ++i)
			{
				T*& value = values[i];
				if (value)
				{
					ShowObject2(object, propertyData, &value); // todo jpp : this breaks if we use this to replace a child in an array of parent type with another child?
				}
			}
		}

		if (!hasOpenPopup && popupIsOpen)
		{
			ImGui::OpenPopup("Select file to add");
			popupIsOpen = false;
			//ptrDisplay.SetOpenPopup(false);
		}

		if (ImGui::BeginPopupModal("Select file to add", NULL, 0))
		{
			ImGui::Text("Select a file from the list:");
			// todo jpp : support inheritance
			// todo jpp : this could be cleaned up into a "select lx file modal popup" all fancy
			{
				const std::vector<FileDisplayInformation>& allFiles = g_ObjectManager->GetAllFiles();
				std::vector<FileDisplayInformation> filteredFiles;
				// keep only files with compatible type
				// todo jpp : fix this for inheritance, perf
				for (const FileDisplayInformation& file : allFiles)
				{
					if (file.m_Typename == T::ClassName)
					{
						filteredFiles.push_back(file);
					}
				}

				int numItems = filteredFiles.size();
				//int selectedFileIndex = imguiManager.GetSelectedFileIndex();
				ImGui::ListBox("Symbols", &selectedFileIndex, VectorOfFileDisplayInfoGetter, (void*)&filteredFiles, numItems);
				//imguiManager.SetSelectedFileIndex(selectedFileIndex);

				if (ImGui::Button("Ok"))
				{
					LXObject* newObject = g_ObjectManager->GetObjectFromIDAndType(true, filteredFiles[selectedFileIndex].m_ObjectID, filteredFiles[selectedFileIndex].m_Typename);
					values.push_back((T*)newObject);

					//imguiManager.SetSelectedFileIndex(-1);
					selectedFileIndex = -1;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					//imguiManager.SetSelectedFileIndex(-1);
					selectedFileIndex = -1;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		return success;

	}

	template <typename T, typename U>
	bool ShowProperty(std::map<U, char *>* map, const char* name);

	template <typename T, typename U>
	bool ShowProperty(std::map<U, T*>* map, const char* name);

	template <typename T, typename U>
	bool ShowProperty(std::unordered_map<U, char *>* map, const char* name);

	template <typename T, typename U>
	bool ShowProperty(std::unordered_map<U, T*>* map, const char* name);

	void ShowVariableAsText(glm::vec3 variable, const char* variableName);
	void ShowVariableAsText(glm::vec3* variable, const char* variableName);
	void ShowVariableAsText(float variable, const char* variableName);
	void ShowVariableAsText(int variable, const char* variableName);
	void ShowGUIText(const char* text);
	void ShowGUIText(std::string* text);
	void ShowGUIText(std::string* text, const char* variableName);
	void ShowGUIText(const std::string& text);
	void ShowGUIText(const std::string& text, const char* variableName);

	bool VectorOfFileDisplayInfoGetter(void* data, int n, const char** out_text);


}

class LXImguiWidthScope
{
public:
	LXImguiWidthScope(int width = 100);
	~LXImguiWidthScope();
};