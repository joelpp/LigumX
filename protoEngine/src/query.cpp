#include "LigumX.h"

using namespace std;
using namespace glm;



vec2 LigumX::windowPosToWorldPos(vec2 ij){
    //if viewsize == x, you see between -x and x
    //Find position according to the window, between -1 and 1
    vec2 relativeToWindow;
    relativeToWindow.x = 2*(ij.x / renderer.windowWidth)-1;
    relativeToWindow.y = 2*(ij.y / renderer.windowHeight)-1;

    //Required because maps have y going up while the screen has y going down
    relativeToWindow.y *= -1;

    //World position is then the camera position + the relative
    //widnow position, scaled by the viewsize.
    vec2 worldPos;
    worldPos = vec2(camera->position.x,camera->position.y) + relativeToWindow * camera->viewSize;

    return worldPos;

}

std::pair<vec2, vec2> LigumX::windowBoundingBox(){
    vec2 bottomLeft;
    vec2 topRight;

    vec2 center = vec2(camera->position.x, camera->position.y);
    float offset = atan(camera->totalViewAngleY/2.f) * camera->position.z;

    bottomLeft = center - vec2(offset,offset);
    topRight = center + vec2(offset,offset);

    return std::pair<vec2, vec2>(bottomLeft, topRight);
}

