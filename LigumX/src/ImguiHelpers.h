#pragma once

#include "LXSystem.h"
#include "imgui.h"


// todo : overload macros based on number of arguments? 

#define LXIMGUI_SHOW_BOOL(name, value) ImguiHelpers::ShowBool(name, value)
#define LXIMGUI_SHOW_INT(name, value, min, max) ImguiHelpers::ShowInt(name, value, min, max)
#define LXIMGUI_SHOW_FLOAT(name, value, min, max) ImguiHelpers::ShowFloat(name, value, min, max)
#define LXIMGUI_SHOW_OBJECTPTR(name, value) // todo : implement this

namespace ImguiHelpers
{
	bool ShowBool(const char* name, bool& value);
	bool ShowBool(LXString& name, bool& value);

	bool ShowInt(const char* name, int& value, int min, int max);
	bool ShowInt(LXString& name, int& value, int min, int max);

	bool ShowFloat(const char* name, float& value, float min, float max);
	bool ShowFloat(LXString& name, float& value, float min, float max);



}