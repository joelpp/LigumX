#include "game.h"
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;

void Game::insertDebugMessage(string message, GLenum severity, GLuint id)
{
    // TODO: make glDebug work.
//    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR,
//                         id, severity, GLsizei(message.length()), message.c_str());
    cout << message << endl;
}

//void Game::debugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei length, const GLchar* message, const void *)
//{
//    cout << "message :"  << endl;
//    for(int i=0; i<length; ++i) {
//        cout << message[i] << endl;
//    }
//}

void Game::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    game->running = false;
}

void Game::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
    if(!TwEventMouseWheelGLFW(yOffset)) {
        static const float factor = 1.1;
        if(yOffset < 0) {
            game->camera->multViewSizeBy(factor);
        } else {
            game->camera->multViewSizeBy(1.f/factor);
        }
    }
}

void Game::glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if(!TwEventKeyGLFW(key, action)) {
        if(action == GLFW_PRESS){
            if (key == GLFW_KEY_SPACE) { game->showTweakBar = !game->showTweakBar; }
            if (key == GLFW_KEY_ESCAPE) {
                if(game->camera->controlType == Camera::ControlType::QWEASDZXC_CONTINUOUS) game->camera->controlType = Camera::ControlType::QWEASDZXC_DRAG;
            }
            if (key == GLFW_KEY_R) { game->init_pipelines(); }
        }
        game->camera->handlePresetKey(pWindow, key, scancode, action, mods);
    }
}

void Game::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{

    if(!TwEventMouseButtonGLFW(button, action)) {
        // Left-click
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
            double x,y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 worldPos = game->windowPosToWorldPos(vec2(x,y));
            int index = 0;
            std::vector<Way*> closests;
            std::vector<double> distances;
            std::vector<vec2> directions;
            vector<std::pair<Node*, Node*>> nodePairs;
            int filter = OSMElement::CONTOUR;
            TIME(closests = game->findNClosestWays(2, worldPos, filter, distances, directions, nodePairs));

            if (closests[0] == NULL) return;
            TIME(game->updateSelectedWay(closests[0]));
            PRINTELEMENTVECTOR(closests);
        }
        //Right Click
        else if (button == GLFW_MOUSE_BUTTON_2){
//            if (action == GLFW_PRESS){
//                game->draggingCamera = true;
//                double x; double y;
//                glfwGetCursorPos(pWindow, &x, &y);
//                game->oldMousePosition = vec2(x,y);
//            }
//            else if (action == GLFW_RELEASE){
//                game->draggingCamera = false;
//            }
        }

        game->camera->handlePresetMouseButton(pWindow, button, action, mods);
    }
}

void Game::glfwMousePositionCallback(GLFWwindow* pWindow, double x, double y)
{
    if(!TwEventMousePosGLFW(x, y)) {
        if (game->draggingCamera){
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            vec2 offset = vec2(x,y) - game->oldMousePosition;
            offset.y *= -1; // reversed controls? this should be an option

            game->camera->translateBy(vec3(offset/1000.f,0));

            game->oldMousePosition = vec2(x,y);

        }
        game->camera->handlePresetCursorPos(pWindow, x, y);
    }
}
