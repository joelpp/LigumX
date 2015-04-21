#version 430 core
in layout(location=0) vec3 pos;
in layout(location=1) vec2 texCoord;
in int gl_VertexID;

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
