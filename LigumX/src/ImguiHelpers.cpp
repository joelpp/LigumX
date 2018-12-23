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


bool ImguiHelpers::ShowFloat(const char* name, float& value, float min, float max)
{
	return ImGui::DragFloat(name, &value, 1.f, min, max);
}

bool ImguiHelpers::ShowFloat(LXString& name, float& value, float min, float max)
{
	return ImguiHelpers::ShowFloat(name.c_str(), value, min, max);
}
