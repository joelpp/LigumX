#version 410 core
in /*layout(location=0)*/ vec3 pos;
in /*layout(location=1)*/ float distance;
uniform mat4 vpMat;
out gl_PerVertex {
    vec4 gl_Position;
};
out float vDistance;
void main() {
    gl_Position = vec4(pos.xyz, 1);
    vDistance = distance;
}