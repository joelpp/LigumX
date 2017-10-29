#include "stdafx.h"

#include "LigumX.h"
#include "World.h"
#include "SectorManager.h"
#include "Renderer.h"
#include "EditorOptions.h"
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace glm;

Renderer* LigumX::m_Renderer;

void LigumX::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    LigumX::GetInstance().Shutdown();
}

void LigumX::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
        static const float factor = 1.1;
		Camera* cam = m_Renderer->GetDebugCamera();
        if(yOffset < 0) 
		{
			cam->SetViewSize(factor * cam->GetViewSize());
        } 
		else 
		{
			cam->SetViewSize(cam->GetViewSize() * (1.f / factor));
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
			m_Renderer->GetEditorOptions()->SetEnabled(!m_Renderer->GetEditorOptions()->GetEnabled());
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
		float add = 2.f * (int)(action == GLFW_PRESS) - 1;
		m_Renderer->AddToXYZMask(add * glm::vec3(key == GLFW_KEY_X, key == GLFW_KEY_Y, key == GLFW_KEY_Z));
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
					m_Renderer->SetMouseButton1Down(true);
					m_Renderer->SetMouseClickPosition(glm::vec2(x, y));
				}
				else if (action == GLFW_RELEASE)
				{
					m_Renderer->SetMouseButton1Down(false);
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

			m_Renderer->SetMousePosition(glm::vec2(x, y));
        }

		m_Renderer->GetDebugCamera()->handlePresetCursorPos(pWindow, x, y);
}
