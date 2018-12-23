#include "ImguiHelpers.h"

bool ImguiHelpers::ShowBool(const char* name, bool& value)
{
	return ImGui::Checkbox(name, &value);
}

bool ImguiHelpers::ShowBool(LXString& name, bool& value)
{
	return ImguiHelpers::ShowBool(name.c_str(), value);
}


bool ImguiHelpers::ShowInt_NoLimits(const char* name, int& value)
{
	return ImGui::DragInt(name, &value);
}
bool ImguiHelpers::ShowInt_NoLimits(LXString& name, int& value)
{
	return ShowInt_NoLimits(name.c_str(), value);
}


bool ImguiHelpers::ShowFloat_NoLimits(const char* name, float& value)
{
	return ImGui::DragFloat2(name, &value);
}

bool ImguiHelpers::ShowFloat_NoLimits(LXString& name, float& value)
{
	return ImguiHelpers::ShowFloat_NoLimits(name.c_str(), value);
}
