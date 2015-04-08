#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

// 2D orthographic camera, look in the -Z direction.
class Camera
{
public:
    glm::vec3 position;
    glm::mat4 vpMat;
    float viewSize;
    float angle;
    Camera();
    void translateBy(glm::vec3 delta);
    void translateTo(glm::vec3 position);
    void moveFromUserInput(GLFWwindow* pWindow);
    void updateMVPMatrix();
    void setViewSize(float inViewSize);
    void multViewSizeBy(float factor);
    void rotate(float _angle);
};

#endif // CAMERA_H
