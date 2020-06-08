#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in float distance;

out gl_PerVertex {
    vec4 gl_Position;
};

out float vDistance;

void main() {
    gl_Position = vec4(pos.xyz, 1);
    vDistance = distance;
}