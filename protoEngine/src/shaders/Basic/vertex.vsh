#version 410 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

uniform mat4 vpMat;
out vec2 myTexCoord;
out float height;
out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = vpMat * vec4(pos, 1);
    myTexCoord = texCoord;
    height = pos.z;
}
