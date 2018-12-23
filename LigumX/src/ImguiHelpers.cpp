#include "ImguiHelpers.h"

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

bool ImguiHelpers::ShowLong(const char* name, long& value, long min, long max)
{
	// todo fix this...
	return ImGui::DragInt(name, (int*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowLong(LXString& name, long& value, long min, long max)
{
	return ShowLong(name.c_str(), value, min, max);
}

bool ImguiHelpers::ShowIVec2(const char* name, glm::ivec2& value, int min, int max)
{
	return ImGui::DragInt2(name, (int*)&value, 1.f, min, max);
}

bool ImguiHelpers::ShowIVec2(LXString& name, glm::ivec2& value, int min, int max)
{
	return ShowIVec2(name.c_str(), value, min, max);
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
