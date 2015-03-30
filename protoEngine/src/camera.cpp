#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

Camera::Camera()
{
    position = vec2(0,0);
    vpMat = mat4(1);
    viewSize = 1;
}

void Camera::translateBy(vec2 delta)
{
    position += delta;
    updateVPMatrix();
}

void Camera::translateTo(vec2 inPosition)
{
    position = inPosition;
    updateVPMatrix();
}

void Camera::setViewSize(float inViewSize)
{
    viewSize = inViewSize;
    updateVPMatrix();
}

void Camera::multViewSizeBy(float factor)
{
    viewSize *= factor;
    updateVPMatrix();
}

void Camera::moveFromUserInput(GLFWwindow *pWindow)
{
    static const float deltaFactor = 0.05;

    if( glfwGetKey(pWindow, GLFW_KEY_S)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_DOWN)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec2(0,-1));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_W)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_UP)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec2(0,1));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_A)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_LEFT)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec2(-1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_D)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_RIGHT)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec2(1,0));
    }
}

void Camera::updateVPMatrix() {
    vpMat = ortho(-viewSize, viewSize, -viewSize, viewSize) *
            translate(mat4(1), -vec3(position.x, position.y, 0));
}
