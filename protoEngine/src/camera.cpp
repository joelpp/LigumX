#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

Camera::Camera()
{
    position = vec3(0,0,0);
    vpMat = mat4(1);
    viewSize = 1;
    angle = 0;
}

void Camera::translateBy(vec3 delta)
{
    position += delta;
    updateMVPMatrix();
}

void Camera::translateTo(vec3 inPosition)
{
    position = inPosition;
    updateMVPMatrix();
}

void Camera::setViewSize(float inViewSize)
{
    viewSize = inViewSize;
    updateMVPMatrix();
}

void Camera::multViewSizeBy(float factor)
{
    viewSize *= factor;
    updateMVPMatrix();
}

void Camera::rotate(float _angle){
    angle += _angle;
    updateMVPMatrix();
}

void Camera::moveFromUserInput(GLFWwindow *pWindow)
{
    static const float deltaFactor = 0.05;

    if( glfwGetKey(pWindow, GLFW_KEY_S)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_DOWN)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(0,-1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_W)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_UP)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(0,1,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_A)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_LEFT)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(-1,0,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_D)==GLFW_PRESS ||
        glfwGetKey(pWindow, GLFW_KEY_RIGHT)==GLFW_PRESS ) {
        translateBy(deltaFactor * viewSize * vec3(1,0,0));
    }
    if( glfwGetKey(pWindow, GLFW_KEY_R)==GLFW_PRESS ) {
        rotate(0.01);
    }
    if( glfwGetKey(pWindow, GLFW_KEY_T)==GLFW_PRESS ) {
        rotate(-0.01);
    }
}

void Camera::updateMVPMatrix() {
    vpMat = ortho(-viewSize, viewSize, -viewSize, viewSize) *
            glm::rotate(mat4(1), angle, vec3(0,0,1)) *
            translate(mat4(1), -vec3(position.x, position.y, 0));


}
