#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>

#define PI (3.14159265359)

using namespace glm;

Camera::Camera()
{
    mvpMat = mat4(1);
    viewSize = 1;
    angle = 0;
    totalViewAngleY = 45;
    aspectRatio = 1; // TODO: change to window's aspect ratio.
    near = 0.000001f;
    far = 1.f;

    cameraType = CameraType::CYLINDRICAL;
    controlType = ControlType::QWEASDZXC_DRAG;
    upVec = normalize(vec3(0,1,1));
    frontVec = normalize(vec3(0,-1,1));
    rightVec = vec3(1,0,0);
    cylindricalUpVec = vec3(0,0,1);
    upVecReference = upVec;
    frontVecReference = frontVec;
    rightVecReference = rightVec;
//    translateTo(vec3(45.47500,-73.65500,10000));
//    scaleTo(vec3(1,1,1));
//    lookAtTargetPos = vec3(45.47500,-73.65500,0);
    lookAtTargetPos = vec3(0,0,0);
    defaultViewMovementSpeed = 0.1f;
    viewMovementSpeed = defaultViewMovementSpeed;
    defaultKeyMovementSpeed = 0.0001f;
    keyMovementSpeed = defaultKeyMovementSpeed;
    keyMovementSpeedIncreaseFactor = 1.5f;
    mouseIsDragging = false;
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

void Camera::updateMVPMatrix()
{
//    vpMat = ortho(-viewSize, viewSize, -viewSize, viewSize) *
//            glm::rotate(mat4(1), angle, vec3(0,0,1)) *
//            translate(mat4(1), -vec3(position.x, position.y, 0));

    mvpMat = column(mat4(1), 0, vec4(rightVec, 0));
    mvpMat = column(mvpMat, 1, vec4(upVec, 0));
    mvpMat = column(mvpMat, 2, vec4(frontVec, 0));
    mvpMat = column(mvpMat, 3, vec4(0, 0, 0, 1));
    mvpMat = translate(position) * mvpMat;
    mvpMat = inverse(mvpMat);
//    mvpMat = translate(position) * mvpMat;

    //mvpMat = perspective((float)PI/8.0f, 1.0f, 0.01f, 1000.0f) * mvpMat;
    //mvpMat = perspective((float)45, 1.0f, 0.0001f, 1.f) * mvpMat;
    mvpMat = perspective(totalViewAngleY, aspectRatio, near, far) * mvpMat;
//    mvpMat = perspective((float)PI/3.0f, 1.0f, 0.01f, 1000.0f) * mvpMat;


//    static unsigned int count = 0;
//    if(++count % 100 == 0) {
//        count = 0;
//        std::cout << "=================" << std::endl;
//        std::cout << position.x << " " << position.y << " " << position.z << std::endl;
//        std::cout << frontVec.x << " " << frontVec.y << " " << frontVec.z << std::endl;
//        std::cout << upVec.x << " " << upVec.y << " " << upVec.z << std::endl;
//    }

}








void Camera::handlePresetMouseButton(
        GLFWwindow *pWindow, int button, int action, int mods)
{
//    if(mbUsePresetMouseButtonBehavior) {
        switch(controlType) {
        case ControlType::QWEASDZXC_DRAG:
            dragMousePresetButton(pWindow, button, action, mods);
        default:
            break;
        }
//    }
}


void Camera::dragMousePresetButton(GLFWwindow* pWindow, int button, int action, int mods)
{
    (void)mods;
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(action == GLFW_PRESS) {
            positionReference = position;
            mouseIsDragging = true;
            double xx, yy;
            glfwGetCursorPos(pWindow, &xx, &yy);
            mouseDragReferencePosition = vec2(xx, yy);
            distanceReference = distance(position, lookAtTargetPos);
            frontVecReference = frontVec;
            upVecReference = upVec;
            rightVecReference = rightVec;
//            mbRotationMatrixIsDirty = true;
            updateMVPMatrix();
        } else if(action == GLFW_RELEASE) {
           mouseIsDragging = false;
            if(cameraType == CameraType::TOP_3D) {
                frontVec = vec3(0,0,1);
                upVec = vec3(0,1,0);
                rightVec = vec3(1,0,0);
            } else {
                frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
                if(cameraType == CameraType::CYLINDRICAL) {
                    rightVec = normalize(cross(cylindricalUpVec, frontVec));
                    upVec = cross(frontVec, rightVec);
                } else {
                    upVec = vec3(tempExtraRotationMat * vec4(upVecReference,1));
                    rightVec = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
                }
                if(cameraType == CameraType::AROUND_TARGET) {
                    position = lookAtTargetPos + distanceReference*frontVec;
                }
//                mbTranslationMatrixIsDirty = true;
//                mbRotationMatrixIsDirty = true;
            }
            updateMVPMatrix();
        }
    }
}


void Camera::handlePresetCursorPos(GLFWwindow *pWindow, double x, double y)
{
//    if(mbUsePresetCursorPosBehavior) {
        //vec2 cursorPos;
        switch(controlType) {
        case ControlType::QWEASDZXC_DRAG:
            dragMousePresetCursorPos(pWindow, x, y);
            break;
        case ControlType::QWEASDZXC_CONTINUOUS:
            //cursorPos = pWindow->getCursorPos();
            //continuousMousePresetCursorPos(pWindow, cursorPos.x, cursorPos.y);
            continuousMousePresetCursorPos(pWindow, x, y);
            qweasdzxcKeyHoldPreset(pWindow);
            break;
        default:
            break;
        }
//    }
}

void Camera::dragMousePresetCursorPos(GLFWwindow* /*pWindow*/, double x, double y)
{
    if(mouseIsDragging) {
        vec2 deltaPos = vec2(x, y) - mouseDragReferencePosition;
        if(cameraType == CameraType::AROUND_TARGET) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        upVecReference);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
            frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
            upVec = vec3(tempExtraRotationMat * vec4(upVecReference,1));
            rightVec = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
            position = lookAtTargetPos + distanceReference*frontVec;
//            mbRotationMatrixIsDirty = true;
//            mbTranslationMatrixIsDirty = true;
        } else if(cameraType == CameraType::CYLINDRICAL) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        cylindricalUpVec);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
            frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
            rightVec = normalize(cross(cylindricalUpVec, frontVec));
            upVec = cross(frontVec, rightVec);
//            mbRotationMatrixIsDirty = true;
        } else if(cameraType == CameraType::AIRPLANE) {
            tempExtraRotationMat = glm::rotate(mat4(1.0),
                                        -deltaPos.x*viewMovementSpeed,
                                        upVecReference);
            tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                        mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVecReference);
            frontVec = vec3(tempExtraRotationMat * vec4(frontVecReference,1));
            upVec = vec3(tempExtraRotationMat * vec4(upVecReference,1));
            rightVec = vec3(tempExtraRotationMat * vec4(rightVecReference,1));
//            mbRotationMatrixIsDirty = true;
        } else if(cameraType == CameraType::TOP_3D) {
            frontVec = vec3(0,0,1);
            upVec = vec3(0,1,0);
            rightVec = vec3(1,0,0);
            position = positionReference - viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
        }
    }
}



void Camera::handlePresetKey(
        GLFWwindow *pWindow, int key, int scancode, int action, int mods)
{
//    if(mbUsePresetKeyBehavior) {
        switch(controlType) {
        case ControlType::QWEASDZXC_DRAG:
        case ControlType::QWEASDZXC_CONTINUOUS:
        case ControlType::QWEASDZXC_ARROWS:
            qweasdzxcKeyPreset(pWindow, key, scancode, action, mods);
            break;
        default:
            break;
        }
//    }
}

void Camera::qweasdzxcKeyPreset(
        GLFWwindow *pWindow, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    (void)pWindow;

    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch(key) {
        case GLFW_KEY_Z:
            keyMovementSpeed /= keyMovementSpeedIncreaseFactor;
            break;
        case GLFW_KEY_X:
            keyMovementSpeed = defaultKeyMovementSpeed;
            break;
        case GLFW_KEY_C:
            keyMovementSpeed *= keyMovementSpeedIncreaseFactor;
            break;
        }
    }
}


void Camera::handlePresetNewFrame(GLFWwindow* pWindow)
{
//    if(mbUsePresetNewFrameBehavior) {
        //vec2 cursorPos;
        switch(controlType) {
        case ControlType::QWEASDZXC_DRAG:
        case ControlType::QWEASDZXC_CONTINUOUS:
            qweasdzxcKeyHoldPreset(pWindow);
            break;
        case ControlType::QWEASDZXC_ARROWS:
            qweasdzxcKeyHoldPreset(pWindow);
            viewArrowsPreset(pWindow);
            break;
        default:
            break;
        }
//    }
}


void Camera::viewArrowsPreset(GLFWwindow* pWindow)
{
    vec2 deltaPos = vec2(0,0);
    if(glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS) {
        deltaPos = deltaPos - vec2(0,1);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        deltaPos = deltaPos + vec2(0,1);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
        deltaPos = deltaPos - vec2(1,0);
    }
    if(glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        deltaPos = deltaPos + vec2(1,0);
    }
    if(cameraType == CameraType::AROUND_TARGET) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
        position = lookAtTargetPos + (distance(lookAtTargetPos,position))*frontVec;
//        mbRotationMatrixIsDirty = true;
//        mbTranslationMatrixIsDirty = true;
    } else if(cameraType == CameraType::CYLINDRICAL) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    cylindricalUpVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        rightVec = normalize(cross(cylindricalUpVec, frontVec));
        upVec = cross(frontVec, rightVec);
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::AIRPLANE) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::TOP_3D) {
        frontVec = vec3(0,0,1);
        upVec = vec3(0,1,0);
        rightVec = vec3(1,0,0);
        position += viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
    }
}


void Camera::continuousMousePresetCursorPos(
        GLFWwindow *pWindow, double x, double y)
{
    //vec2 deltaPos = vec2(x, y) - vec2(pWindow->getCenter());
    int windowWidth, windowHeight;
    glfwGetWindowSize(pWindow, &windowWidth, &windowHeight);
    vec2 deltaPos = vec2(x, y) - 0.5f*vec2(float(windowWidth), float(windowHeight));
    if(cameraType == CameraType::AROUND_TARGET) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
        position = lookAtTargetPos + (distance(lookAtTargetPos,position))*frontVec;
//        mbRotationMatrixIsDirty = true;
//        mbTranslationMatrixIsDirty = true;
    } else if(cameraType == CameraType::CYLINDRICAL) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    cylindricalUpVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        rightVec = normalize(cross(cylindricalUpVec, frontVec));
        upVec = cross(frontVec, rightVec);
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::AIRPLANE) {
        tempExtraRotationMat = glm::rotate(mat4(1.0),
                                    -deltaPos.x*viewMovementSpeed,
                                    upVec);
        tempExtraRotationMat = tempExtraRotationMat * glm::rotate(
                    mat4(1.0), -deltaPos.y*viewMovementSpeed, rightVec);
        frontVec = vec3(tempExtraRotationMat * vec4(frontVec,1));
        upVec = vec3(tempExtraRotationMat * vec4(upVec,1));
        rightVec = vec3(tempExtraRotationMat * vec4(rightVec,1));
//        mbRotationMatrixIsDirty = true;
    } else if(cameraType == CameraType::TOP_3D) {
        frontVec = vec3(0,0,1);
        upVec = vec3(0,1,0);
        rightVec = vec3(1,0,0);
        position += viewMovementSpeed*vec3(deltaPos.x, -deltaPos.y, 0);
    }
//    vec2 centerPos = vec2(pWindow->getCenter());
    glfwSetCursorPos(pWindow, float(windowWidth)/2.f, float(windowHeight)/2.f);
}

void Camera::qweasdzxcKeyHoldPreset(GLFWwindow *pWindow)
{
    if(cameraType == CameraType::AROUND_TARGET) {
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
            position -= keyMovementSpeed * frontVec;
            distanceReference = max(distanceReference - keyMovementSpeed, 0.0f);
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
            position += keyMovementSpeed * frontVec;
            distanceReference = max(distanceReference + keyMovementSpeed, 0.0f);
        }
    } else if(cameraType == CameraType::TOP_3D) {
        frontVec = vec3(0,0,1);
        upVec = vec3(0,1,0);
        rightVec = vec3(1,0,0);
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
            position += keyMovementSpeed * upVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
            position -= keyMovementSpeed * upVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
            position -= keyMovementSpeed * rightVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
            position += keyMovementSpeed * rightVec;
        }
    } else {
        if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
            position -= keyMovementSpeed * frontVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
            position += keyMovementSpeed * frontVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
            position -= keyMovementSpeed * rightVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
            position += keyMovementSpeed * rightVec;
        }
        if(glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS) {
            if(cameraType == CameraType::CYLINDRICAL) {
                position -= keyMovementSpeed * cylindricalUpVec;
            } else {
                position -= keyMovementSpeed * upVec;
            }
        }
        if(glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS) {
            if(cameraType == CameraType::CYLINDRICAL) {
                position += keyMovementSpeed * cylindricalUpVec;
            } else {
                position += keyMovementSpeed * upVec;
            }
        }
    }
    updateMVPMatrix();
}
