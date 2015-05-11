#version 410 core
layout(location=0) in vec3 pos;
//layout(location=1) in vec3 color;
// uniform vec3 color;
uniform mat4 vpMat;
out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
};
// out vec3 vColor;
void main() {
	gl_PointSize = 10.0;
    gl_Position = vpMat * vec4(pos, 1);
    // vColor = color;
}