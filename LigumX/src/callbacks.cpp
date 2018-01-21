#include "stdafx.h"

#include "LigumX.h"
#include "World.h"
#include "SectorManager.h"
#include "Renderer.h"
#include "Editor.h"
#include "EditorOptions.h"
#include "InputHandler.h"
#include <string>
#include <chrono>


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
	ImGui_ImplGlfwGL3_ScrollCallback(pWindow, xOffset, yOffset);

	bool caughtByImgui = ImGui::IsMouseHoveringAnyWindow();

	if (!caughtByImgui)
	{
		g_Editor->AddToTerrainBrushSize((float) yOffset);
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

			g_Editor->SetTerrainErasureMode(g_IsCtrlHeld);
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
		g_InputHandler->HandleInput(pWindow, button, action, mods);
	}

  //      // Left-click
  //      if (button == GLFW_MOUSE_BUTTON_1)
		//{
		//	double x, y;
		//	glfwGetCursorPos(pWindow, &x, &y);

		//	if (!caughtByImgui)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			g_InputHandler->SetMouse1Pressed(true);
		//			g_Editor->SetMouseClickPosition(glm::vec2(x, y));
		//		}
		//		else if (action == GLFW_RELEASE)
		//		{
		//			g_Editor->SetMouseButton1Down(false);
		//		}
		//	}
  //      }
  //      //Right Click
  //      else if (button == GLFW_MOUSE_BUTTON_2){
            if (action == GLFW_PRESS)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else if (action == GLFW_RELEASE)
			{
				glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        //}

		m_Renderer->GetDebugCamera()->handlePresetMouseButton(pWindow, button, action, mods);
}

void LigumX::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
        if (true)
		{
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);

			g_InputHandler->SetMousePosition(glm::vec2(x, y));
        }

		m_Renderer->GetDebugCamera()->handlePresetCursorPos(pWindow, x, y);
}
