#version 430 core
layout(lines_adjacency) in;
in gl_PerVertex {
    vec4 gl_Position;
} gl_in[];
//in float vDistance[];

uniform mat4 vpMat;

layout(points, max_vertices=2) out;
out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
};
//out vec2 gTexCoord;

void main() {

    float streetWidth = 0.0001;

    // generate primitives
    vec3 p;
    p = gl_in[1].gl_Position.xyz;
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
//    gTexCoord = vec2(0,0);
    gl_PointSize = length((vpMat*vec4(streetWidth/2.0 * 800,0,0,0)).xy);
//    gl_PointSize = streetWidth/2.0 * 1000;
    EmitVertex();
    EndPrimitive();
    p = gl_in[2].gl_Position.xyz ;
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
//    gTexCoord = vec2(0,0);
    gl_PointSize = length((vpMat*vec4(streetWidth/2.0 * 800,0,0,0)).xy);
//    gl_PointSize = streetWidth/2.0 * 1000;
    EmitVertex();
    EndPrimitive();

}
