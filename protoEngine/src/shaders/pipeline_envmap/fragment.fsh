#version 430 core

in vec4 gl_FragCoord;

uniform vec2 windowSize;
uniform vec3 sunDir; // assumed to be normalized
uniform vec3 viewDir; // assumed to be normalized
uniform vec3 viewRight; // assumed to be normalized
uniform vec3 viewUp; // assumed to be normalized
uniform vec2 viewAngles; // total view angle in x and y, in radian
uniform float viewNear;

out vec3 color;

//// globals
//vec3 viewRight;
//vec3 viewUp;

//mat4 rotationMatrix(vec3 axis, float angle)
//{
//    // taken from http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
//    axis = normalize(axis);
//    float s = sin(angle);
//    float c = cos(angle);
//    float oc = 1.0 - c;

//    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
//                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
//                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
//                0.0,                                0.0,                                0.0,                                1.0);
//}

// gives the direction of the ray that passes through the screen at pos01.
vec3 screenToVec(vec2 pos01)
{
    vec2 pos11 = 2.0*(pos01-vec2(0.5,0.5));
    vec3 res =   viewDir * viewNear
               + pos11.x * viewNear*tan(viewAngles.x/2.0) * viewRight
               + pos11.y * viewNear*tan(viewAngles.y/2.0) * viewUp;
    return normalize(res);
}

void main() {

    vec2 pos01 = gl_FragCoord.xy/windowSize;
    vec3 fragDir = screenToVec(pos01);
    float cosAngleToSun = dot(fragDir, sunDir);
    float cosAngleToWorldUp = dot(fragDir, vec3(0,0,1));

    if(cosAngleToWorldUp > 0) {
        float cosSun01 = 0.5*(cosAngleToSun+1);
        color =   0.7*vec3(0.41, 0.78, 0.86)
                + pow(cosSun01,512.0)*vec3(1,1, 1)
                + 0.3*pow(cosSun01,1.0)*vec3(1, 1, 1);
    } else {
        color = vec3(0.25);
    }
}
