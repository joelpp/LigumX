#pragma once

#include "LXSystem.h"
#include "imgui.h"

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

	bool ShowObjectPtr(const char* name, glm::vec3* value);
	bool ShowObjectPtr(const char* name, glm::vec2* value);
	bool ShowObjectPtr(const char* name, int* value);

	template <typename T>
	bool ShowObjectPtr(const char* name, T* value)
	{
		bool isNull = (value == nullptr);
		char treeNodeName[256];
		sprintf(treeNodeName, "%s (%s)%s", name, "[fixThis]"/*value->ClassName*/, (isNull ? " (nullptr)" : ""));

		if (value == nullptr)
		{
			ShowRawString(treeNodeName);
		}
		else
		{
			ImguiTreeNodeScope scope(treeNodeName);

			bool success = scope.m_Opened;

			if (success)
			{
				success = value->ShowPropertyGrid();
			}

			return success;
		}

		return true;
	}

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

	bool ShowProperty(void* object, const ClassPropertyData& propertyData, glm::ivec2* value, float min, float max);
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, std::string* value);

	template <typename T>
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, T* value)
	{
		return false;
	}
	template <typename T>
	bool ShowProperty(void* object, const ClassPropertyData& propertyData, std::vector<T*> value)
	{
		return false;
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



}