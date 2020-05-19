

#include <string>
#include <chrono>

#include "LigumX.h"
#include "World.h"
#include "SectorManager.h"
#include "Renderer.h"
#include "GUI.h"

#include "Editor.h"
#include "EditorOptions.h"
#include "TerrainTool.h"

#include "InputHandler.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace glm;

Renderer* LigumX::m_Renderer;

bool g_IsShiftHeld = false;
bool g_IsCtrlHeld = false;

void LigumX::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
	LigumX::GetInstance().Shutdown();
}

void LigumX::glfwMouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
	bool caughtByGUI = g_GUI->ProcessMouseScroll((float) xOffset, (float) yOffset);

	if (!caughtByGUI)
	{
		g_InputHandler->AddTo_MouseScroll(glm::vec2(xOffset, yOffset));
	}
}

void flipBool(bool& value)
{
	value = !value;
}

void LigumX::HandleKeyboardInput(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	g_InputHandler->HandleKeyboardInput(key, scancode, action, mods);

	g_Editor->HandleInputEvent(key, action, mods);

	// send event to entity Manager (temporary before a playerInput class)
	LigumX& game = LigumX::GetInstance();

    if(action == GLFW_PRESS){
        if (key == GLFW_KEY_SPACE) 
		{ 
			g_Editor->GetOptions()->SetEnabled(!g_Editor->GetOptions()->GetEnabled());
		}
        else if (key == GLFW_KEY_ESCAPE) 
		{
			if (m_Renderer->GetDebugCamera()->controlType == Camera::ControlType::QWEASDZXC_CONTINUOUS)
			{
				m_Renderer->GetDebugCamera()->controlType = Camera::ControlType::QWEASDZXC_DRAG;
			}
        }
		else if (key == GLFW_KEY_R)
		{ 
			m_Renderer->InitPipelines(); 
		}
		else if (key == GLFW_KEY_M)
		{
            for( auto it = m_Renderer->displayElementType.begin(); it != m_Renderer->displayElementType.end(); ++it)
			{
                it->second = false;
            }
        }
		else if(key == GLFW_KEY_F4)
		{
        }
    }

	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		bool isPress = (action == GLFW_PRESS);
		bool isShift = (key == GLFW_KEY_LEFT_SHIFT);
		bool isCtrl =  (key == GLFW_KEY_LEFT_CONTROL);
		bool isXYZ[3] = { key == GLFW_KEY_X, key == GLFW_KEY_Y, key == GLFW_KEY_Z };

		if (isShift)
		{
			g_IsShiftHeld = isPress ? true : false;
		}
		else if (isCtrl)
		{
			g_IsCtrlHeld = isPress ? true : false;
		}

		bool maskModified = isXYZ[0] || isXYZ[1] || isXYZ[2] || isShift;

		if (maskModified)
		{
			float add = isPress ? 1.f : -1.f;

			glm::vec3 newMask = glm::vec3(g_Editor->GetXYZMask());

			newMask = add * glm::vec3(isXYZ[0], isXYZ[1], isXYZ[2]) + newMask;
			newMask = glm::max(newMask, glm::vec3(-1, -1, -1));
			newMask = glm::min(newMask, glm::vec3(1, 1, 1));

			g_Editor->SetXYZMask(glm::vec4(newMask, g_IsShiftHeld));
		}

	}

	m_Renderer->GetDebugCamera()->handlePresetKey(pWindow, key, scancode, action, mods);

	ImGui_ImplGlfwGL3_KeyCallback(pWindow, key, scancode, action, mods);


}

void LigumX::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
	bool caughtByImgui = ImGui::IsMouseHoveringAnyWindow() && !g_InputHandler->GetDragging();

	if (!caughtByImgui)
	{
		g_InputHandler->HandleMouseButtonInput(pWindow, button, action, mods);
		m_Renderer->GetDebugCamera()->handlePresetMouseButton(pWindow, button, action, mods);
	}

}

void LigumX::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
	{
		double x; double y;
		glfwGetCursorPos(pWindow, &x, &y);

		g_InputHandler->SetMousePosition(glm::vec2(x, y));
	}

	lxAssert(m_Renderer->GetDebugCamera() != nullptr);

	m_Renderer->GetDebugCamera()->handlePresetCursorPos(pWindow, x, y);
}
