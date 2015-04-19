#version 430 core

in vec4 gl_FragCoord;

uniform vec2 windowSize;
uniform float sunOrientation; // angle from (1,0,0) around (0,0,1)
uniform float sunTime;
uniform vec3 viewDir; // assumed to be normalized
uniform vec3 viewRight; // assumed to be normalized
uniform vec3 viewUp; // assumed to be normalized
uniform vec2 viewAngles; // total view angle in x and y, in radian
uniform float viewNear;

out vec3 color;

float pi = 3.14159265359;
vec3 sunDir;
vec2 sunDirFlat;

// adapted from http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat3 rotateAroundAxis(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c          );
}

// artificial noise in [0,1]
float noise(vec3 v)
{
    float a = sin(430.123*v.x*v.y*sin(v.z)) + cos(123.65445*v.y*cos(v.x) + 1.543*v.z);
    return 0.5*(1+sin(a*a + a*93721.547));
}

float stars(vec3 v)
{
    v = rotateAroundAxis(vec3(-sunDirFlat.y, sunDirFlat.x,0), sunTime) * v;
    v = rotateAroundAxis(vec3(0,0,1), sunOrientation) * v;


    vec3 temp = v;
    temp = floor(400.0*temp)/400.0 ;
    if(noise(temp) < 0.00001) return 1;
    else return 0;
}


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

    // compute globals
    sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
    sunDir = cos(sunTime)*vec3(0,0,1) + sin(sunTime)*vec3(sunDirFlat.x, sunDirFlat.y, 0);

    vec2 pos01 = gl_FragCoord.xy/windowSize;
    vec3 fragDir = screenToVec(pos01);
    float cosAngleToSun = dot(fragDir, sunDir);
    float cosAngleToWorldUp = dot(fragDir, vec3(0,0,1));

    if(cosAngleToWorldUp > 0) {
        float cosSun01 = 0.5*(cosAngleToSun+1);

        float sunsetFact =  smoothstep(-0.2, 0.05, sunDir.z)
                           *smoothstep( 0.3, 0.05, sunDir.z);
//        float overGroundBool = float(sunDir.z > 0);
        float overGroundBool = smoothstep(-0.05, 0.05, sunDir.z);
        int moonBool = int(cosSun01 < 0.001);

        color =
            sunsetFact *(
                (0.5+0.5*pow(cosSun01,1))*vec3(245.0/255.0, 98.0/255.0, 35.0/255.0)
                + 0.75*pow(cosSun01,64.0)*vec3(1,1, 1)
            )
            +
            (1.0-sunsetFact) * overGroundBool * (
                  0.7*vec3(0.41, 0.78, 0.86)
                + pow(cosSun01,512.0)*vec3(1,1, 1)
                + 0.3*pow(cosSun01,1.0)*vec3(1, 1, 1)
            )
            +
            (1.0-sunsetFact) * (1-overGroundBool) * (
                  moonBool * vec3(0.75)
                + (1-moonBool) * vec3( stars(fragDir) )
//                + (1-moonBool) * vec3( stars(fragDir, -sunDir) )
            )
        ;

    } else {
        color = vec3(0.25);
    }
}
