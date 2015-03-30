#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

// 2D orthographic camera, look in the -Z direction.
class Camera
{
public:
    glm::vec2 position;
    glm::mat4 vpMat;
    float viewSize;

    Camera();
    void translateBy(glm::vec2 delta);
    void translateTo(glm::vec2 position);
    void moveFromUserInput(GLFWwindow* pWindow);
    void updateVPMatrix();
    void setViewSize(float inViewSize);
    void multViewSizeBy(float factor);
};

#endif // CAMERA_H
