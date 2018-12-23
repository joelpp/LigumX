#pragma once

#include "LXSystem.h"
#include "imgui.h"

#define LXIMGUI_SHOW_BOOL(name, value) ImguiHelpers::ShowBool(name, value)
#define LXIMGUI_SHOW_INT(name, value) ImguiHelpers::ShowInt_NoLimits(name, value)
#define LXIMGUI_SHOW_FLOAT(name, value) ImguiHelpers::ShowFloat_NoLimits(name, value)
#define LXIMGUI_SHOW_FLOAT_LIMITS(name, value, min, max) ImguiHelpers::ShowFloat(name, value, min, max)
#define LXIMGUI_SHOW_OBJECTPTR(name, value) // todo : implement this

namespace ImguiHelpers
{
	bool ShowBool(const char* name, bool& value);
	bool ShowBool(LXString& name, bool& value);

	bool ShowInt_NoLimits(const char* name, int& value);
	bool ShowInt_NoLimits(LXString& name, int& value);

	bool ShowFloat_NoLimits(const char* name, float& value);
	bool ShowFloat_NoLimits(LXString& name, float& value);



}