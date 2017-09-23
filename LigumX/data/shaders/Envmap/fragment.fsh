#version 410 core

in vec4 gl_FragCoord;
in vec3 direction;
in vec3 normal;
in vec2 uv;

uniform vec2 windowSize;
uniform float sunOrientation; // angle from (1,0,0) around (0,0,1)
uniform float sunTime;
uniform vec3 viewDir; // assumed to be normalized
uniform vec3 viewRight; // assumed to be normalized
uniform vec3 viewUp; // assumed to be normalized
uniform vec2 viewAngles; // total view angle in x and y, in radian
uniform float viewNear;

#define PROVIDER_View

#ifdef PROVIDER_View
uniform vec3 g_CameraPosition;
uniform float g_CameraNearPlane;
uniform float g_CameraFarPlane;
uniform mat4 g_ViewToWorldMatrix;
uniform mat4 g_ProjectionMatrixInverse;
#endif




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
    //vec2 pos11 = 2.0*(pos01-vec2(0.5,0.5));
    //vec3 res =   viewDir * viewNear
    //           + pos11.x * viewNear*tan(viewAngles.x/2.0) * viewRight
    //           + pos11.y * viewNear*tan(viewAngles.y/2.0) * viewUp;
    //return normalize(res);

	//vec3 a = vec3(2.0*(gl_FragCoord.xy - vec2(0.5,0.5), gl_FragCoord.z));
	//vec3 ndc = a * gl_FragCoord.w;

	   //vec4 ndc = vec4(
    //    (gl_FragCoord.x /*/ windowSize.x*/ - 0.5) * 2.0,
    //    (gl_FragCoord.y /*/ windowSize.y*/ - 0.5) * 2.0,
    //    (gl_FragCoord.z - 0.5) * 2.0,
    //    1.0);

	vec4 viewport = vec4(0.f, 0.f, windowSize.x, windowSize.y);

	vec4 ndcPos;
	ndcPos.xy = ((2.0 * gl_FragCoord.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1;
	ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
		(gl_DepthRange.far - gl_DepthRange.near);
	ndcPos.w = 1.0;

	vec4 clipPos = ndcPos / gl_FragCoord.w;

	vec4 viewPos = g_ProjectionMatrixInverse*  clipPos ;
	vec4 worldPos = g_ViewToWorldMatrix* viewPos ;

	return  normalize(viewPos.xyz/* - g_CameraPosition*/);
}

void main() {

    // compute globals
    sunDirFlat = vec2(cos(sunOrientation), sin(sunOrientation));
    sunDir = cos(sunTime)*vec3(0,0,1) + sin(sunTime)*vec3(sunDirFlat.x, sunDirFlat.y, 0);

    //vec3 pos01 = vec3(gl_FragCoord.xy, 0/*gl_FragCoord.z*/);
	 vec2 pos01 = gl_FragCoord.xy/windowSize;
    vec3 fragDir = normalize(direction);

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
