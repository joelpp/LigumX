#include "LigumX.h"
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace glm;


void LigumX::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    Game->running = false;
}

void LigumX::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
    if(!TwEventMouseWheelGLFW(yOffset)) {
        static const float factor = 1.1;
        if(yOffset < 0) {
            Game->camera->multViewSizeBy(factor);
        } else {
            Game->camera->multViewSizeBy(1.f/factor);
        }
    }
}

void LigumX::glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if(!TwEventKeyGLFW(key, action)) {
        // send event to entity Manager (temporary before a playerInput class)
        Game->entityManager.KeyCallback(key, action);

        if(action == GLFW_PRESS){
            if (key == GLFW_KEY_SPACE) { Game->showTweakBar = !Game->showTweakBar; }
            if (key == GLFW_KEY_ESCAPE) {
                if(Game->camera->controlType == Camera::ControlType::QWEASDZXC_CONTINUOUS) Game->camera->controlType = Camera::ControlType::QWEASDZXC_DRAG;
            }
            if (key == GLFW_KEY_R) { Game->renderer.init_pipelines(); }
            if (key == GLFW_KEY_M) {
                for( auto it = Game->renderer.displayElementType.begin(); it != Game->renderer.displayElementType.end(); ++it){
                    it->second = false;
                }
            }

            if(key == GLFW_KEY_F4) {
                Game->toggleEntityLand();
            }

        }
        Game->camera->handlePresetKey(pWindow, key, scancode, action, mods);
    }
}

void LigumX::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{

    if(!TwEventMouseButtonGLFW(button, action)) {
        // Left-click
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
            // double x,y;
            // glfwGetCursorPos(pWindow, &x, &y);
            // vec2 worldPos = Game->windowPosToWorldPos(vec2(x,y));
            // int index = 0;
            // std::vector<Way*> closests;
            // std::vector<double> distances;
            // std::vector<vec2> directions;
            // vector<std::pair<Node*, Node*>> nodePairs;
            // int filter = OSMElement::CONTOUR;
            // TIME(closests = Game->findNClosestWays(2, worldPos, filter, distances, directions, nodePairs));

            // if (closests[0] == NULL) return;
            // TIME(Game->updateSelectedWay(closests[0]));
            // PRINTELEMENTVECTOR(closests);
        }
        //Right Click
        else if (button == GLFW_MOUSE_BUTTON_2){
//            if (action == GLFW_PRESS){
//                Game->draggingCamera = true;
//                double x; double y;
//                glfwGetCursorPos(pWindow, &x, &y);
//                Game->oldMousePosition = vec2(x,y);
//            }
//            else if (action == GLFW_RELEASE){
//                Game->draggingCamera = false;
//            }
        }

        Game->camera->handlePresetMouseButton(pWindow, button, action, mods);
//        TODO
        //        Game->renderer->updateMVPMatrix();
    }
}

void LigumX::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
    if(!TwEventMousePosGLFW(x, y)) {
        if (Game->draggingCamera){
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 offset = vec2(x,y) - Game->oldMousePosition;
            offset.y *= -1; // reversed controls? this should be an option

            Game->camera->translateBy(vec3(offset/1000.f,0));

            Game->oldMousePosition = vec2(x,y);

        }
        Game->camera->handlePresetCursorPos(pWindow, x, y);
    }
}
