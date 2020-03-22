#include "ImguiHelpers.h"
#include <glm/gtx/string_cast.hpp>

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


bool ImguiHelpers::ShowProperty(float* value, const char* name, float min, float max)
{
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		return ImGui::DragFloat(name, (float*)value);
	}
	else
	{
		return ImGui::SliderFloat(name, value, min, max);
	}

}

bool ImguiHelpers::ShowProperty(glm::vec2* value, const char* name, float min, float max)
{
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		return ImGui::DragFloat2(name, (float*)value);
	}
	else
	{
		return ImGui::SliderFloat2(name, (float*)value, min, max);
	}
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


	return false;
}

bool ImguiHelpers::ShowProperty(glm::vec4* value, const char* name, float min, float max)
{
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		return ImGui::DragFloat4(name, (float*)value);
	}
	else
	{
		return ImGui::SliderFloat4(name, (float*)value, min, max);
	}
}

bool ImguiHelpers::ShowProperty(glm::ivec2* value, const char* name, float min, float max)
{
	if (min == LX_LIMITS_FLOAT_MIN && max == LX_LIMITS_FLOAT_MAX)
	{
		return ImGui::DragInt2(name, (int*)value);
	}
	else
	{
		return ImGui::SliderInt2(name, (int*)value, (int)min, (int)max);
	}
}

bool ImguiHelpers::ShowProperty(std::string* value, const char* name)
{
	//ShowGUIText(value, name);
	//return false;
	return false;
}

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
