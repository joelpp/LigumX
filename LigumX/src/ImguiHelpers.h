#pragma once

#include "LXSystem.h"
#include "imgui.h"


// todo : overload macros based on number of arguments? 

#define LXIMGUI_SHOW_BOOL(name, value) ImguiHelpers::ShowBool(name, value)
#define LXIMGUI_SHOW_INT(name, value, min, max) ImguiHelpers::ShowInt(name, value, min, max)
#define LXIMGUI_SHOW_FLOAT(name, value, min, max) ImguiHelpers::ShowFloat(name, value, min, max)
#define LXIMGUI_SHOW_VEC2(name, value, min, max)
#define LXIMGUI_SHOW_VEC3(name, value, min, max)
#define LXIMGUI_SHOW_VEC4(name, value, min, max)
#define LXIMGUI_SHOW_STRING(name, value)

#define LXIMGUI_SHOW_OBJECTREF(name, value, type) ImguiHelpers::ShowObjectPtr(name, value);

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

namespace ImguiHelpers
{
	bool ShowBool(const char* name, bool& value);
	bool ShowBool(LXString& name, bool& value);

	bool ShowInt(const char* name, int& value, int min, int max);
	bool ShowInt(LXString& name, int& value, int min, int max);

	bool ShowFloat(const char* name, float& value, float min, float max);
	bool ShowFloat(LXString& name, float& value, float min, float max);

	bool ShowVec2(const char* name, glm::vec2& value, float min, float max);
	bool ShowVec2(LXString& name, glm::vec2& value, float min, float max);

	bool ShowVec3(const char* name, glm::vec3& value, float min, float max);
	bool ShowVec3(LXString& name, glm::vec3& value, float min, float max);

	bool ShowVec4(const char* name, glm::vec4& value, float min, float max);
	bool ShowVec4(LXString& name, glm::vec4& value, float min, float max);

	bool BeginPropertyTree(const char* name);

	void EndPropertyTree();

	template <typename T>
	bool ShowObjectPtr(const char* name, T* value)
	{
		ImguiIDScope idScope(name);

		bool success = false;
		if (success = BeginPropertyTree(name))
		{
			success = value->ShowPropertyGrid();
			EndPropertyTree();
		}

		return success;
	}

}