#version 430 core

in vec4 gl_FragCoord;

uniform vec2 windowSize;
uniform vec3 sunDir;

out vec3 color;

void main() {
    color = vec3(gl_FragCoord.xy/windowSize, 0);
    //color = vec3(1,0, 0);
}
