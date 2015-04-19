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

float pi = 3.14159265359;

// artificial noise in [0,1]
float noise(vec3 v)
{
    float a = sin(430.123*v.x) + cos(123.65445*v.y*v.z);
    return 0.5*(1+sin(a*a + a*93721.547));
}

//// somewhat structured noise
//float smoothNoise(vec3 v)
//{
//    float a = 0;
//    for(int i=0; i<15; i++) {
////        a +=   sin(noise(vec3(1*i,1*i,1*i))*i*i*v.x + noise(vec3(7*i,1*i,4*i)*i*v.y))
////             + sin(noise(vec3(2*i,1*i,5*i))*i*i*v.y + noise(vec3(8*i,3*i,3*i)*i*v.z))
////             + sin(noise(vec3(1*i,3*i,2*i))*i*i*v.z + noise(vec3(5*i,4*i,3*i)*i*v.x));

//        a +=   noise(vec3(1*i,1*i,1*i))*sin(noise(vec3(1*i,7*i,1*i))*i*i*v.x + noise(vec3(3*i,2*i,5*i))*i*i*v.y)
//             + noise(vec3(2*i,1*i,5*i))*sin(noise(vec3(2*i,1*i,5*i))*i*i*v.y + noise(vec3(3*i,1*i,7*i))*i*i*v.z)
//             + noise(vec3(1*i,3*i,2*i))*sin(noise(vec3(1*i,3*i,2*i))*i*i*v.z + noise(vec3(1*i,7*i,1*i))*i*i*v.x);
//    }
//    return 0.5*(1+sin(a));
//}






// need an orientation here, because we have a singularity in the direction opposite to the ori.
float stars(vec3 v, vec3 ori)
{
    // choose a vector perpendicular to ori. We take two candidates and keep the
    // one that is furtest to ori, and then orthogonalize it.
    float dotA = dot(vec3(1,0,0), ori);
    float dotB = dot(vec3(0,1,0), ori);
    vec3 perpY;
    if(abs(dotA) < abs(dotB)) {
        perpY = vec3(1,0,0);
    } else {
        perpY = vec3(0,1,0);
    }
    perpY = cross(perpY, ori);
    perpY = normalize(cross(ori, perpY));
    vec3 perpX = normalize(cross(perpY, ori));

    float phi = acos(dot(v, ori));
    vec3 vecFlat = normalize(v-cos(phi)*ori);
    float theta = acos(dot(vecFlat, perpX)) * sign(dot(vecFlat, perpY));

//    vec2 planar;
//    if(abs(phi) < pi/4 || abs(phi-pi) < pi/4) {
//        planar = sin(phi)*vec2(cos(theta),sin(theta));
//    } else {
//        planar = vec2(1,1);
//    }

//    for(int i=1; i<4; i++) {
//        phi = sin(phi*2.1234*pi*i);
//    }

//    return phi;


//    return 0.5*(sin(500*(1+0.5*sin(15*planar.x))*planar.x) + sin(500*planar.y));

    //return 0.5*(cos(30*theta) + cos(50*sqrt(1-phi/(pi/2)*phi/(pi/2))));
    //return 0.5*(cos(30*theta) + cos(50*acos(2*phi/(pi)-1)));
//    return clamp(0.5*(cos(30*theta) + cos(50*phi)), 0, 1);
//    return clamp(0.5*(cos(100*theta) + cos(100*acos(2*phi/(pi/2.0)-1))), 0, 1);
//    return clamp(0.5*(cos(75*theta) + cos(200*phi*sqrt(sin(phi)))), 0, 1);
//    return clamp(0.5*(cos(50*theta) + cos(100*acos(2*(phi/pi)-1))), 0, 1);
//    return clamp(0.5*(cos(50*smoothNoise(vec3(theta))) + cos(200*smoothNoise(vec3(phi)))), 0, 1);



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

    vec2 pos01 = gl_FragCoord.xy/windowSize;
    vec3 fragDir = screenToVec(pos01);
    float cosAngleToSun = dot(fragDir, sunDir);
    float cosAngleToWorldUp = dot(fragDir, vec3(0,0,1));

    if(cosAngleToWorldUp > 0) {
        float cosSun01 = 0.5*(cosAngleToSun+1);

        float sunsetFact =  smoothstep(-0.2, 0.05, sunDir.z)
                           *smoothstep( 0.3, 0.05, sunDir.z);
        float overGroundBool = float(sunDir.z > 0);
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
                + (1-moonBool) * vec3( pow(stars(fragDir, -sunDir),2) )
//                + (1-moonBool) * vec3( stars(fragDir, -sunDir) )
            )
        ;

    } else {
        color = vec3(0.25);
    }
}
