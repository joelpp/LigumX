#version 410 core
layout(lines_adjacency) in;
in gl_PerVertex {
    vec4 gl_Position;
} gl_in[];
in float vDistance[];

uniform mat4 vpMat;

layout(triangle_strip, max_vertices=4) out;
//layout(line_strip, max_vertices=4) out;
out gl_PerVertex
{
  vec4 gl_Position;
};
out vec2 gTexCoord;

void main() {

    float streetWidth = 0.00001;

    // find intersection point with previous line
    vec2 v2 = normalize((gl_in[2].gl_Position - gl_in[1].gl_Position).xy);
    vec2 v1 = normalize((gl_in[1].gl_Position - gl_in[0].gl_Position).xy);
    vec2 r2 = normalize(vec2(v2.y, -v2.x));
    vec2 r1 = normalize(vec2(v1.y, -v1.x));
    vec2 rPrev = normalize(r1+r2);
    float d = dot(-v1,rPrev);
    rPrev = rPrev * streetWidth/2.0 / sqrt(1-d*d);

    // find intersection point with next line
    v2 = normalize((gl_in[3].gl_Position - gl_in[2].gl_Position).xy);
    v1 = normalize((gl_in[2].gl_Position - gl_in[1].gl_Position).xy);
    r2 = normalize(vec2(v2.y, -v2.x));
    r1 = normalize(vec2(v1.y, -v1.x));
    vec2 rNext = normalize(r1+r2);
    d = dot(-v1,rNext);
    rNext = rNext * streetWidth/2.0 / sqrt(1-d*d);

    // generate primitives
    vec3 p;
    p.xy = gl_in[1].gl_Position.xy + rPrev;
    p.z = gl_in[1].gl_Position.z;
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);
    gTexCoord = vec2(1,0);
    EmitVertex();

    p.xy = gl_in[2].gl_Position.xy + rNext;
    p.z = gl_in[2].gl_Position.z;
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);

    gTexCoord = vec2(1,1);
    EmitVertex();

    p.xy = gl_in[1].gl_Position.xy - rPrev;
    p.z = gl_in[1].gl_Position.z;
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);

    gTexCoord = vec2(0,0);
    EmitVertex();
    p.xy = gl_in[2].gl_Position.xy - rNext;
    p.z = gl_in[2].gl_Position.z;
    gl_Position = vpMat*vec4(p.x, p.y, p.z, 1);

    gTexCoord = vec2(0,1);
    EmitVertex();
    EndPrimitive();

}
