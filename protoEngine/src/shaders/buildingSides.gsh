#version 430 core
layout(lines) in;
in gl_PerVertex {
    vec4 gl_Position;
} gl_in[];
in float vDistance[];

uniform mat4 vpMat;
uniform float uBuildingHeight;
uniform float uScaleFactor;

layout(triangle_strip, max_vertices=4) out;
out gl_PerVertex
{
  vec4 gl_Position;
};
out vec2 gTexCoord;

void main() {

//    float buildingHeight = 0.0001;
    float buildingHeight = uBuildingHeight / 10000;

    // generate primitives
    vec3 p;
    float totalHeight = gl_in[0].gl_Position.z + buildingHeight;
    p = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z);
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
    gTexCoord = vec2(vDistance[0]/(buildingHeight)*uScaleFactor,0);

    EmitVertex();
    p = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z);
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
    gTexCoord = vec2(vDistance[1]/(buildingHeight)*uScaleFactor,0);

    EmitVertex();
    p = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, totalHeight);
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
    gTexCoord = vec2(vDistance[0]/(buildingHeight)*uScaleFactor,uScaleFactor);

    EmitVertex();
    p = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, totalHeight);
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
    gTexCoord = vec2(vDistance[1]/(buildingHeight)*uScaleFactor,uScaleFactor);
    EmitVertex();

    EndPrimitive();
}
