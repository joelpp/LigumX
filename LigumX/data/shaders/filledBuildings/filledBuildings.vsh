#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in float id;
uniform mat4 vpMat;
out gl_PerVertex {
    vec4 gl_Position;
};
out vec2 texCoord;
out float ID; 
void main() {
    gl_Position = vpMat * vec4(pos.x,pos.y, pos.z / 10000.0, 1);
    texCoord = pos.xy;
    ID = id;
}