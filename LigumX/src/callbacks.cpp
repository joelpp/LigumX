#include "stdafx.h"

#include "LigumX.h"
#include "World.h"
#include "SectorManager.h"
#include "Renderer.h"
#include "Editor.h"
#include "EditorOptions.h"
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace glm;

Renderer* LigumX::m_Renderer;

bool g_IsShiftHeld = false;

void LigumX::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    LigumX::GetInstance().Shutdown();
}

void LigumX::glfwMouseScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
	ImGui_ImplGlfwGL3_ScrollCallback(pWindow, xOffset, yOffset);

	bool caughtByImgui = ImGui::IsMouseHoveringAnyWindow();

	if (!caughtByImgui)
	{
		g_Editor->AddToTerrainBrushSize(yOffset);
	}

}

void flipBool(bool& value)
{
	value = !value;
}

void LigumX::HandleKeyboardInput(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	
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
			m_Renderer->init_pipelines(); 
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
		else if (key == GLFW_KEY_L)
		{
			game.world->m_sectorManager->setLoadNewSectors(game.world->m_sectorManager->getLoadNewSectors());
		}
    }

	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		bool isShift = (key == GLFW_KEY_LEFT_SHIFT);
		bool isXYZ[3] = { key == GLFW_KEY_X, key == GLFW_KEY_Y, key == GLFW_KEY_Z };

		if (isShift)
		{
			g_IsShiftHeld = (action == GLFW_PRESS) ? true : false;
		}
		else
		{
			float add = 2.f * (int)(action == GLFW_PRESS) - 1;
			add *= g_IsShiftHeld ? -1 : 1;

			g_Editor->AddToXYZMask(add * glm::vec3(key == GLFW_KEY_X, key == GLFW_KEY_Y, key == GLFW_KEY_Z));
		}

	}

	m_Renderer->GetDebugCamera()->handlePresetKey(pWindow, key, scancode, action, mods);

	ImGui_ImplGlfwGL3_KeyCallback(pWindow, key, scancode, action, mods);
}

void LigumX::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
	bool caughtByImgui = ImGui::IsMouseHoveringAnyWindow();
        // Left-click
        if (button == GLFW_MOUSE_BUTTON_1)
		{
			double x, y;
			glfwGetCursorPos(pWindow, &x, &y);

			if (!caughtByImgui)
			{
				if (action == GLFW_PRESS)
				{
					g_Editor->SetMouseButton1Down(true);
					g_Editor->SetMouseClickPosition(glm::vec2(x, y));
				}
				else if (action == GLFW_RELEASE)
				{
					g_Editor->SetMouseButton1Down(false);
				}
			}
        }
        //Right Click
        else if (button == GLFW_MOUSE_BUTTON_2){
            if (action == GLFW_PRESS)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else if (action == GLFW_RELEASE)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

		m_Renderer->GetDebugCamera()->handlePresetMouseButton(pWindow, button, action, mods);
}

void LigumX::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
        if (true)
		{
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);

			g_Editor->SetMousePosition(glm::vec2(x, y));
        }

		m_Renderer->GetDebugCamera()->handlePresetCursorPos(pWindow, x, y);
}
