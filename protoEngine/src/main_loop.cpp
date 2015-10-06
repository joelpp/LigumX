#include "LigumX.h"
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace glm;
using std::cout;
using std::endl;

const double static_dt = 1.0 / 100.0;

void LigumX::mainLoop()
{
    static double dt = 0.0;
//    static double curr_time = glfwGetTime();
    static double physic_accumulator = 0.0;

    physic_accumulator += dt;

    camera->handlePresetNewFrame(renderer.pWindow);

    renderer.render();

    if(showTweakBar) TwDraw();
}

